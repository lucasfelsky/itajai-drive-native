// Renderer 5.0 final HDR presentation pass.
// Exposure is intentionally temporal rather than an hour-boundary switch: dusk,
// dawn, weather and lightning converge at different rates to avoid visible pops.
// Q0 caps bloom below the shader's wide-kernel threshold, preserving the cheaper
// local bloom path when the adaptive performance system is already under stress.
static float g_renderer500_exposure=1.06f;
static float renderer500_smooth01(float x){x=clampf(x,0,1);return x*x*(3.0f-2.0f*x);}
static float renderer500_night_factor(void){float h=(float)g_hour+g_clock_accum60/60.0f;if(h>=19.1f||h<4.9f)return 1.0f;if(h>=17.25f){return renderer500_smooth01((h-17.25f)/(19.1f-17.25f));}if(h<6.55f){return 1.0f-renderer500_smooth01((h-4.9f)/(6.55f-4.9f));}return 0.0f;}
static void renderer500_end_scene(void){
 if(!g_renderer500_ready||!g_renderer500_hdr||!g_post500)return;
 p500BindFramebuffer(GL_FRAMEBUFFER,0);glViewport(0,0,g_w,g_h);glDisable(GL_DEPTH_TEST);glDisable(GL_BLEND);p09UseProgram(g_post500);
 p500ActiveTexture(GL_TEXTURE0);glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,g_color500);p09Uniform1i(u500_scene,0);
 p500ActiveTexture(GL_TEXTURE1);glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,g_depth500);p09Uniform1i(u500_depth,1);
 PFN09UNIFORM2F u2=(PFN09UNIFORM2F)wglGetProcAddress("glUniform2f");if(u2&&u500_inv>=0)u2(u500_inv,1.0f/maxf(1,(float)g_renderer500_w),1.0f/maxf(1,(float)g_renderer500_h));
 float night=renderer500_night_factor(),weather=g_weather==1?.025f:(g_weather>=2?.070f:0.0f);float target=1.045f+night*.195f+weather;target*=1.0f-.13f*clampf(g_lightning60,0,1);float dt=1.0f/maxf(20.0f,g_renderer500_last_fps),rate=target<g_renderer500_exposure?7.0f:2.15f;g_renderer500_exposure=lerpf(g_renderer500_exposure,target,clampf(dt*rate,0,1));
 p09Uniform1f(u500_exp,g_renderer500_exposure);p09Uniform1f(u500_fx,g_renderer500_fxaa?1.0f:0);p09Uniform1f(u500_ao,g_renderer500_ao?.92f:0);
 float bloom=.46f+night*.58f+clampf(g_wetness60,0,1)*.18f;if(g_weather>=2)bloom+=.07f;if(g_perf80_quality==0)bloom=minf(bloom,.69f);else if(g_perf80_quality==1)bloom=minf(bloom,.96f);p09Uniform1f(u500_bloom,g_renderer500_bloom?bloom:0);p09Uniform1f(u500_wet,clampf(g_wetness60,0,1));p09Uniform1f(u500_time,(float)g_time);
 glMatrixMode(GL_PROJECTION);glPushMatrix();glLoadIdentity();glMatrixMode(GL_MODELVIEW);glPushMatrix();glLoadIdentity();glBegin(GL_QUADS);glTexCoord2f(0,0);glVertex2f(-1,-1);glTexCoord2f(1,0);glVertex2f(1,-1);glTexCoord2f(1,1);glVertex2f(1,1);glTexCoord2f(0,1);glVertex2f(-1,1);glEnd();glPopMatrix();glMatrixMode(GL_PROJECTION);glPopMatrix();glMatrixMode(GL_MODELVIEW);
 p09UseProgram(0);p500ActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,0);glDisable(GL_TEXTURE_2D);p500ActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,0);glDisable(GL_TEXTURE_2D);glEnable(GL_DEPTH_TEST);
}
