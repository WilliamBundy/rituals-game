
void render_start(Render_Group* group);
bool render_has_clip_rect(Render_Group* group);
void render_set_clip_rect(Render_Group* group, real x, real y, real w, real h);
void render_sort(Render_Group* group, isize offset);
void render_add(Render_Group* group, Sprite* sprite);
void render_add_unclipped(Render_Group* group, Sprite* sprite);
void render_add(Render_Group* group, Sprite4* s4);
void render_calculate_ortho_matrix(real* ortho, Vec4 screen, real nearplane, real farplane);
void render_draw(OpenGL_Renderer* r, Render_Group* group, Vec2 size, real scale);

void render_start();
bool render_has_clip_rect();
void render_set_clip_rect(real x, real y, real w, real h);
void render_sort(isize offset);
void render_add(Sprite* sprite);
void render_add_unclipped(Sprite* sprite);
void render_add(Sprite4* s4);
void render_draw();
