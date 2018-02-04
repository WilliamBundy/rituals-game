
#define BodyShapeCount 4
#ifndef WirmphtEnabled
struct SimShape
{
	i32 flags, index;
	Vec2 pos, size;
	f32 staticFriction, dynamicFriction;
	f32 restitution;
	SimBody* body;
};

struct SimContact
{
	Vec2 overlap;
	SimShape *a, *b;
};

struct SimBody
{
	i32 flags, shapeCount;
	Vec2 pos, vel, acl;
	f32 invMass;
	i32 index;
	SimShape* shapes[4];
};

struct SimWorld
{
	MemoryArena* arena;
	MemoryPool* bodyPool;
	MemoryPool* shapePool;
	SimShape** shapes;
	SimBody** bodies;
	isize bodyCount, bodyCapacity;
	isize shapeCount, shapeCapacity;

	f32 testCount, colCount;

	i32 iterations;
	i32 sortAxis;
};
#endif

void simInitBody(SimBody* body)
{
	SimBody nullbody = {0};
	*body = nullbody;
	body->invMass = 1.0f;
	body->index = -1;
}

void simInitShape(SimShape* shape)
{
	SimShape nullshape = {0};
	*shape = nullshape;
	shape->index = -1;
	shape->staticFriction = 0.5f;
	shape->dynamicFriction = 0.25f;
	shape->restitution = 0.33f;

}


SimBody* simGetBody(SimWorld* sim)
{
	SimBody* body = poolRetrieve<SimBody>(sim->bodyPool);
	simInitBody(body);
	return body;
}

void simAddBody(SimWorld* sim, SimBody* body)
{
	//TODO(will) error checking
	body->index = sim->bodyCount;
	sim->bodies[sim->bodyCount++] = body;
	for(isize i = 0; i < body->shapeCount; ++i) {
		if(body->shapes[i]->index == -1) {
			simAddShape(sim, body->shapes[i]);
		}
	}
}

void simRemoveBody(SimWorld* sim, SimBody* body)
{
	for(isize i = 0; i < body->shapeCount; ++i) {
		simRemoveShape(sim, body->shapes[i]);
	}
	SimBody* end = sim->bodies[sim->bodyCount - 1];
	end->index = body->index;
	sim->bodies[body->index] = end;
	sim->bodyCount--;
	poolRelease<SimBody>(sim->bodyPool, body);
}

SimShape* simGetShape(SimWorld* sim)
{
	SimShape* shape = poolRetrieve<SimShape>(sim->shapePool);
	simInitShape(shape);
	return shape;
}

void simAttachShape(SimBody* body, SimShape* shape)
{
	if(body->shapeCount >= BodyShapeCount) return;
	body->shapes[body->shapeCount++] = shape;
	shape->body = body;
}

void simDetachShape(SimShape* shape)
{
	SimBody* body = shape->body;
	isize index = -1;
	for(isize i = 0; i < body->shapeCount; ++i) {
		if(body->shapes[i] == shape) index = i;
	}

	if(index > -1) {
		if(body->shapeCount - 1 > 0) {
			body->shapes[index] = body->shapes[body->shapeCount - 1];
			body->shapes[--body->shapeCount] = NULL;
		} else {
			body->shapes[index] = NULL;
			body->shapeCount = 0;
		}
	}
}

void simAddShape(SimWorld* sim, SimShape* shape)
{
	//TODO(will) error checking
	shape->index = sim->shapeCount;
	sim->shapes[sim->shapeCount++] = shape;
}

void simRemoveShape(SimWorld* sim, SimShape* shape)
{
	simDetachShape(shape);
	SimShape* end = sim->shapes[sim->shapeCount - 1];
	end->index = shape->index;
	sim->shapes[shape->index] = end;
	sim->shapeCount--;
	poolRelease<SimShape>(sim->shapePool, shape);
}

SimBody* simCreateBox(SimWorld* sim, Vec2 pos, Vec2 size)
{
	SimBody* body = simGetBody(sim);
	SimShape* shape = simGetShape(sim);
	simAttachShape(body, shape);
	size *= 0.5;
	body->pos = pos + size;
	body->invMass = 1;
	shape->size = size;
	simAddBody(sim, body);
	return body;
}

SimBody* simAttachBox(SimWorld* sim, SimBody* body, Vec2 pos, Vec2 size)
{
	SimShape* shape = simGetShape(sim);
	simAttachShape(body, shape);
	simAddShape(sim, shape);
	size *= 0.5;
	shape->pos = (pos + size) - body->pos;
	shape->size = size;
	return body;
}


#define ShapeMinSide(s, i) (s->pos.e[i] + s->body->pos.e[i] - s->size.e[i])
#define ShapeMaxSide(s, i) (s->pos.e[i] + s->body->pos.e[i] + s->size.e[i])
void simShapeSort(SimShape** array, isize count, i32 axis)
{
	for(isize i = 1; i < count; ++i) {
		isize j = i - 1;

		f32 minSide = ShapeMinSide(array[i], axis);
		if(ShapeMinSide(array[j], axis) > minSide) {
			SimShape* temp = array[i];
			while((j >= 0) && (ShapeMinSide(array[j], axis) > minSide)) {
				array[j + 1] = array[j];
				j--;
			}
			array[j + 1] = temp;
		}
	}

	for(isize i = 0; i < count; ++i) {
		array[i]->index = i;
	}
}


i32 simIntersectShapes(SimWorld* sim, SimShape* a, SimShape* b, SimContact* c)
{
	if(a->body == b->body) return 0;
	if(a->body->invMass == 0 && b->body->invMass == 0) return 0;

	Vec2 apos = a->pos + a->body->pos;
	Vec2 bpos = b->pos + b->body->pos;
	f32 dx = fabsf(bpos.x - apos.x); 
	f32 sx = a->size.x + b->size.x;
	if(dx > sx) return 0;
	f32 dy = fabsf(bpos.y - apos.y); 
	f32 sy = a->size.y + b->size.y;
	if(dy > sy) return 0;

	sx -= dx;
	sy -= dy;

	if(sx > sy) {
		sx = 0;
		if(apos.y > bpos.y) {
			sy *= -1;
		}
	} else {
		sy = 0;
		if(apos.x > bpos.x) {
			sx *= -1;
		}
	}

	c->overlap = v2(sx, sy);
	c->a = a;
	c->b = b;
	return 1;
}

void simResolveContact(SimWorld* sim, SimContact* c)
{
	SimBody *a = c->a->body, *b = c->b->body;
	f32 invMassSum = a->invMass + b->invMass;
	if(invMassSum == 0) return;
	invMassSum = 1.0f / invMassSum;

	//This works because with AABBs depth is always orthogonal
	//If we add other shapes, this will need to be done properly
	f32 overlap = fabsf(c->overlap.x + c->overlap.y);
	if(overlap <= 0) return;
	Vec2 normal = c->overlap / overlap;
	Vec2 separation = normal * wmaxf(overlap-0.02f, 0) * invMassSum * 0.5;

	if(a->invMass == 0) {
		b->pos += separation;
	} else {
		b->pos += separation * b->invMass;
	}

	if(b->invMass == 0) {
		a->pos -= separation;
	} else {
		a->pos -= separation * a->invMass;
	}
	
	Vec2 relative = b->vel - a->vel;
	f32 velNormal = v2_dot(normal, relative);
	if(velNormal < 0) {
		f32 elasticity = 1.0f + wminf(c->a->restitution, c->b->restitution);
		f32 mag = -1.0f * elasticity * velNormal;
		mag *= invMassSum;
		Vec2 impulse = normal * mag;
		a->vel -= impulse * a->invMass;
		b->vel += impulse * b->invMass;

		Vec2 tangent = relative - normal * velNormal;
		tangent = v2_normalize(tangent);

		f32 frictionMag = -v2_dot(relative, tangent);
		frictionMag *= invMassSum;

		f32 sfA = c->a->staticFriction;
		f32 sfB = c->b->staticFriction;

		f32 staticFriction = sqrtf(sfA * sfA + sfB * sfB);

		Vec2 frictionImpulse = v2(0, 0);
		if(fabsf(frictionMag) < (mag * staticFriction)) {
			frictionImpulse = frictionMag * tangent;
		} else {
			f32 dfA = c->a->dynamicFriction;
			f32 dfB = c->b->dynamicFriction;
			f32 dynamicFriction = sqrtf(dfA * dfA + dfB * dfB);
			frictionImpulse = -mag * tangent * dynamicFriction;
		}

		if(isnan(frictionImpulse.x) || isnan(frictionImpulse.y)) {
			frictionImpulse = v2(0, 0);
		}
		a->vel -= frictionImpulse * a->invMass;
		b->vel += frictionImpulse * b->invMass;

	}
}


SimWorld* simNew(isize capacity)
{
	MemoryInfo mi = getMemoryInfo();
	MemoryArena* arena = arenaBootstrap(mi, 0);
	SimWorld* sim = arenaPush<SimWorld>(arena);
	sim->arena = arena;
	sim->bodyPool = poolBootstrap(mi, sizeof(SimBody), 0);
	sim->shapePool = poolBootstrap(mi, sizeof(SimShape), 0);
	sim->bodyCapacity = capacity;
	sim->bodyCount = 0;
	sim->bodies = arenaPush<SimBody*>(arena, capacity);
	sim->shapes = arenaPush<SimShape*>(arena, capacity);
	sim->iterations = 8;

	sim->testCount = 0;
	sim->colCount = 0;

	arena->flags |= FlagArenaNoRecommit | FlagArenaNoZeroMemory;
	arenaStartTemp(arena);
	return sim;
}

void simUpdate(SimWorld* sim, f32 dt)
{
	arenaEndTemp(sim->arena);
	arenaStartTemp(sim->arena);
	SimContact* contactList = (SimContact*)sim->arena->head;
	isize firstContact = 0;
	isize contactCount = 0;
	isize testCount = 0;

	dt /= sim->iterations;

	for(isize times = 0; times < sim->iterations; ++times) {
		testCount = 0;
		Vec2 variance = v2(0,0), centerSum1 = v2(0,0), centerSum2 = v2(0,0);
		simShapeSort(sim->shapes, sim->shapeCount, sim->sortAxis);
		for(isize i = 0; i < sim->shapeCount; ++i) {
			SimShape* a = sim->shapes[i];
			centerSum1 += a->pos;
			centerSum2 += a->pos * a->pos;
			for(isize j = i+1; j < sim->shapeCount; ++j) {
				SimShape* b = sim->shapes[j];
				if(ShapeMinSide(b, sim->sortAxis) > ShapeMaxSide(a, sim->sortAxis))
					break;
				SimContact c;
				testCount++;
				if(simIntersectShapes(sim, a, b, &c)) {
					SimContact* cc = arenaPush<SimContact>(sim->arena);
					*cc = c;
					contactCount++;
				}
			}
		}

		for(isize i = firstContact; i < contactCount; ++i) {
			simResolveContact(sim, contactList + i);
		}
		printf("Test:%d Contact:%d\r", testCount, contactCount - firstContact);
		firstContact = contactCount;

		for(isize i = 0; i < sim->bodyCount; ++i) {
			SimBody* a = sim->bodies[i];
			a->vel += a->acl * dt;
			a->pos += a->vel * dt;
			a->vel *= 0.95;
		}

		variance = centerSum2 - (centerSum1 * centerSum1) / (f32)sim->shapeCount;
		sim->sortAxis = variance.x > variance.y ? 0 : 1;
	}
}



void simDelete(SimWorld* sim)
{
	arenaDestroy(sim->bodyPool->alloc);
	arenaDestroy(sim->shapePool->alloc);
	arenaDestroy(sim->arena);
}





