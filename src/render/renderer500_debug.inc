// Renderer 5.0 paged debug telemetry.
static const char*renderer500_weather_name(void){return g_weather==0?"LIMPO":g_weather==1?"NUBLADO":g_weather==2?"CHUVA":"TEMPESTADE";}
static const char*renderer500_surface_name(void){return g_player.body.surface==SURFACE_ASPHALT?"ASFALTO":g_player.body.surface==SURFACE_CURB?"MEIO-FIO":"GRAMA";}
static void renderer500_debug_line(float x,float y,const char*label,const char*value,float r,float g,float b){char s[180];sprintf(s,"%-13s %.42s",label,value);gl_text(x,y,s,r,g,b);}
static void renderer500_debug_overlay(void){
  if(!g_debug||g_settings_menu_open||(g_photo70&&g_photo70_hide_hud))return;
  settings_draw_2d_begin();
  char b[220],v[220];float w=minf((float)g_w-96.0f,1120.0f),h=286.0f,x=((float)g_w-w)*.5f,y=42.0f,c2=x+w*.515f;hud_panel(x,y,w,h);
  sprintf(b,"DEBUG %d/3 // %s",g_debug500_page+1,g_debug500_page==0?"GAMEPLAY":g_debug500_page==1?"RENDER + WORLD":"SIM + REDE");gl_text(x+18,y+27,b,.82f,.94f,1.0f);
  gl_text(x+18,y+50,"TAB fecha // SHIFT+TAB pagina",.52f,.68f,.76f);
  float ms=1000.0f/maxf(1.0f,g_renderer500_last_fps);sprintf(b,"%.0f FPS // %.1f ms",g_renderer500_last_fps,ms);gl_text(x+w-145,y+27,b,.62f,.84f,.92f);
  if(g_debug500_page==0){
    renderer500_debug_line(x+18,y+82,"RUA",g_nearest_road,.82f,.88f,.92f);
    renderer500_debug_line(x+18,y+106,"CARRO",g_garage215_names[g_player.car&3],.82f,.88f,.92f);
    sprintf(v,"%02d:00 // %s",g_hour,renderer500_weather_name());renderer500_debug_line(x+18,y+130,"AMBIENTE",v,.72f,.88f,.78f);
    sprintf(v,"%.1f km/h // steer %.2f",absf(g_player.speed)*3.6f,g_player.steer);renderer500_debug_line(x+18,y+154,"MOVIMENTO",v,.82f,.88f,.92f);
    sprintf(v,"%s // camera %d",renderer500_surface_name(),g_player.camera);renderer500_debug_line(x+18,y+178,"SUPERFICIE",v,.72f,.88f,.78f);
    sprintf(v,"%s",g_camera_captured?"CAPTURADO":"LIVRE");renderer500_debug_line(x+18,y+202,"MOUSE",v,g_camera_captured?.50f:.85f,g_camera_captured?1.0f:.80f,.72f);
    sprintf(v,"edges %d // offroute %d",g_gps_n,g_gps_offroute07);renderer500_debug_line(c2,y+82,"GPS",v,.62f,.82f,1.0f);
    sprintf(v,"slip %.3f // lateral %.2f",g_player.slip,g_player.lateralSpeed);renderer500_debug_line(c2,y+106,"ADERENCIA",v,.82f,.88f,.92f);
    sprintf(v,"roll %.3f // pitch %.3f",g_player.roll,g_player.pitch);renderer500_debug_line(c2,y+130,"CARROCERIA",v,.82f,.88f,.92f);
    sprintf(v,"static %d // dynamic %d",g_last_static_contacts,g_last_dynamic_contacts);renderer500_debug_line(c2,y+154,"CONTATOS",v,.82f,.88f,.92f);
    sprintf(v,"traffic %d // avg %.1f km/h",g_traffic_count,g_traffic_avg_speed*3.6f);renderer500_debug_line(c2,y+178,"TRAFEGO",v,.82f,.88f,.92f);
    sprintf(v,"window %s",g_window_active500?"ATIVA":"SEM FOCO");renderer500_debug_line(c2,y+202,"INPUT",v,g_window_active500?.55f:1.0f,g_window_active500?.95f:.62f,.72f);
  }else if(g_debug500_page==1){
    sprintf(v,"HDR %s // LINEAR %s // GL3.3 %s",g_renderer500_hdr?"ON":"FALLBACK",g_renderer09_linear_output?"ON":"OFF",g_renderer500_context33?"COMPAT":"LEGACY");renderer500_debug_line(x+18,y+82,"RENDER",v,.72f,.90f,1.0f);
    sprintf(v,"%dx%d // scale %.2f // Q%d",g_renderer500_w,g_renderer500_h,g_renderer500_scale,g_perf80_quality);renderer500_debug_line(x+18,y+106,"TARGET",v,.82f,.88f,.92f);
    sprintf(v,"AO %s // bloom %s // FXAA %s",g_renderer500_ao?"ON":"OFF",g_renderer500_bloom?"ON":"OFF",g_renderer500_fxaa?"ON":"OFF");renderer500_debug_line(x+18,y+130,"POST",v,.82f,.88f,.92f);
    sprintf(v,"%.2f // wet %.2f",g_renderer500_exposure,g_wetness60);renderer500_debug_line(x+18,y+154,"EXPOSURE",v,.82f,.88f,.92f);
    int cars=renderer500_vehicle_asset_count();const char*vs=g_renderer500_vsync>0?"ON":g_renderer500_vsync==0?"REJECTED":"N/A";sprintf(v,"PAK %s // cars %d/%d",g_asset_pack_ok08?"OK":"MISSING",cars,CAR160_COUNT);renderer500_debug_line(x+18,y+178,"ASSETS",v,cars==CAR160_COUNT?.58f:1.0f,cars==CAR160_COUNT?.86f:.48f,.68f);
    sprintf(v,"VBO %s // VSYNC %s",g_renderer26_vbo_ready?"ON":"FALLBACK",vs);renderer500_debug_line(x+18,y+202,"GPU",v,.82f,.88f,.92f);
    sprintf(v,"roads %d // segs %d",g_world.roadCount,g_world.segCount);renderer500_debug_line(c2,y+82,"WORLD",v,.82f,.88f,.92f);
    sprintf(v,"buildings %d // nodes %d",g_world.buildingCount,g_world.nodeCount);renderer500_debug_line(c2,y+106,"OSM",v,.82f,.88f,.92f);
    sprintf(v,"edges %d",g_world.edgeCount);renderer500_debug_line(c2,y+130,"GRAPH",v,.82f,.88f,.92f);
    sprintf(v,"exact %d // LOD %d",g_perf80_exact_buildings,g_perf80_lod_buildings);renderer500_debug_line(c2,y+154,"BUILDINGS",v,.82f,.88f,.92f);
    sprintf(v,"%.0fm",g_perf80_exact_building_range);renderer500_debug_line(c2,y+178,"EXACT RANGE",v,.82f,.88f,.92f);
    sprintf(v,"%d",g_draw_calls);renderer500_debug_line(c2,y+202,"DRAW CALLS",v,.82f,.88f,.92f);
  }else{
    sprintf(v,"count %d // avg %.1f km/h",g_traffic_count,g_traffic_avg_speed*3.6f);renderer500_debug_line(x+18,y+82,"TRAFEGO",v,.82f,.88f,.92f);
    sprintf(v,"slip %.3f // steer %.2f",g_player.slip,g_player.steer);renderer500_debug_line(x+18,y+106,"FISICA",v,.82f,.88f,.92f);
    sprintf(v,"static %d // dynamic %d",g_last_static_contacts,g_last_dynamic_contacts);renderer500_debug_line(x+18,y+130,"COLISOES",v,.82f,.88f,.92f);
    sprintf(v,"mouse %s // focus %s",g_camera_captured?"CAPTURADO":"LIVRE",g_window_active500?"ON":"OFF");renderer500_debug_line(x+18,y+154,"INPUT",v,.72f,.88f,.78f);
    renderer500_debug_line(x+18,y+178,"WORLD DEBUG","colliders + vehicle debug nesta pagina",.72f,.82f,.90f);
    renderer500_debug_line(x+18,y+202,"ESCOPO","telemetria somente // rede sem mudancas",.52f,.68f,.76f);
    sprintf(v,"%s // %s",net301_mode_name(),g_net301_connected?"CONECTADO":"DESCONECTADO");renderer500_debug_line(c2,y+82,"UDP",v,.62f,.82f,1.0f);
    sprintf(v,"port %d // remote %s",g_net301_port,g_net301_remote.active?"ATIVO":"OFF");renderer500_debug_line(c2,y+106,"PEER",v,.82f,.88f,.92f);
    sprintf(v,"RX %d // TX %d // drop %d",g_net301_rx_packets,g_net301_tx_packets,g_net301_dropped_packets);renderer500_debug_line(c2,y+130,"PACOTES",v,.82f,.88f,.92f);
    sprintf(v,"%s%s",g_net321_hamachi_found?g_net321_hamachi_ip:"nao detectado",g_net321_hamachi_probable?" (provavel)":"");renderer500_debug_line(c2,y+154,"HAMACHI",v,g_net321_hamachi_found?.55f:1.0f,g_net321_hamachi_found?.95f:.62f,.72f);
    renderer500_debug_line(c2,y+178,"STATUS",g_net321_status[0]?g_net321_status:"sem mensagem",.82f,.88f,.92f);
  }
  gl_text(x+18,y+h-18,"Renderer 5.0.2 // TAB substitui HUD legado enquanto aberto",.52f,.68f,.76f);
  settings_draw_2d_end();
}
