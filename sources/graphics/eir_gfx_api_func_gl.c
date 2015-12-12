#include "eir_gfx_api_func.h"
#include "../system/eir_file_system.h"
#include "../kernel/eir_log.h"
#include "eir_image_api_func.h"
#include "eir_gfx_defines.h"

static void eir_gfx_debug_log_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
   EIR_KER_LOG_MESSAGE("%s", "sprite batch info:");
   EIR_KER_LOG_MESSAGE("vbo: %d", batch->vbo);
   EIR_KER_LOG_MESSAGE("vao: %d", batch->vao);
   EIR_KER_LOG_MESSAGE("texture: %d", batch->texture_id[0]);
   EIR_KER_LOG_MESSAGE("sprites count: %d", batch->sprites.used);
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
}

static void eir_gfx_debug_log_vertex_batch(eir_gfx_vertex_batch_t * batch)
{
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
   EIR_KER_LOG_MESSAGE("%s", "vertex batch info:");
   EIR_KER_LOG_MESSAGE("vbo: %d", batch->vbo);
   EIR_KER_LOG_MESSAGE("vao: %d", batch->vao);
   EIR_KER_LOG_MESSAGE("primitive type: %d", batch->primitive_type);
   EIR_KER_LOG_MESSAGE("vertices count: %d", batch->vertices.used);
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
}

void eir_gfx_api_init()
{
   EIR_KER_LOG_MESSAGE("init opengl");
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
}

eir_gfx_api_texture_handle_t eir_gfx_api_create_texture(eir_gfx_image_t * image)
{
   eir_gfx_api_texture_handle_t texture = EIR_GFX_INVALID_TEXTURE_HANDLE;

   if (image)
   {
      EIR_KER_LOG_MESSAGE("create texture image: w=%d, h=%d", image->width, image->height);
      glGenTextures(1, &texture);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexImage2D(
	 GL_TEXTURE_2D,
	 0,
	 GL_RGBA,
	 image->width,
	 image->height,
	 0,
	 GL_BGRA,
	 GL_UNSIGNED_BYTE,
	 image->pixels
	 );
   }
   return texture;
}

void eir_gfx_api_load_sprite_shaders(eir_gfx_env_t * gfx_env)
{
   if (!gfx_env)
   {
      return;
   }
   EIR_KER_LOG_MESSAGE("create sprite shaders");
   gfx_env->sprite_vert_shader = glCreateShader(GL_VERTEX_SHADER);
   eir_gfx_api_compile_shader(gfx_env->sprite_vert_shader, SPRITE_VERTEX_SHADER_PATH);
   gfx_env->sprite_geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
   eir_gfx_api_compile_shader(gfx_env->sprite_geom_shader, SPRITE_GEOMETRY_SHADER_PATH);
   gfx_env->sprite_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
   eir_gfx_api_compile_shader(gfx_env->sprite_frag_shader, SPRITE_FRAGMENT_SHADER_PATH);

   EIR_KER_LOG_MESSAGE("link program");
   gfx_env->sprite_program = glCreateProgram();
   glAttachShader(gfx_env->sprite_program, gfx_env->sprite_vert_shader);
   glAttachShader(gfx_env->sprite_program, gfx_env->sprite_geom_shader);
   glAttachShader(gfx_env->sprite_program, gfx_env->sprite_frag_shader);
   glBindFragDataLocation(gfx_env->sprite_program, 0, "outColor");
   glLinkProgram(gfx_env->sprite_program);
   glDetachShader(gfx_env->sprite_program, gfx_env->sprite_vert_shader);
   glDetachShader(gfx_env->sprite_program, gfx_env->sprite_geom_shader);
   glDetachShader(gfx_env->sprite_program, gfx_env->sprite_frag_shader);
   glUseProgram(0);
}

void eir_gfx_api_load_text_shaders(eir_gfx_env_t * gfx_env)
{
   if (!gfx_env)
   {
      return;
   }

   EIR_KER_LOG_MESSAGE("create text shaders");
   gfx_env->text_vert_shader = glCreateShader(GL_VERTEX_SHADER);
   eir_gfx_api_compile_shader(gfx_env->text_vert_shader, SPRITE_VERTEX_SHADER_PATH);
   gfx_env->text_geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
   eir_gfx_api_compile_shader(gfx_env->text_geom_shader, SPRITE_GEOMETRY_SHADER_PATH);
   gfx_env->text_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
   eir_gfx_api_compile_shader(gfx_env->text_frag_shader, SPRITE_FRAGMENT_SHADER_PATH);
   
   EIR_KER_LOG_MESSAGE("link program");
   gfx_env->text_program = glCreateProgram();
   glAttachShader(gfx_env->text_program, gfx_env->text_vert_shader);
   glAttachShader(gfx_env->text_program, gfx_env->text_geom_shader);
   glAttachShader(gfx_env->text_program, gfx_env->text_frag_shader);
   glBindFragDataLocation(gfx_env->text_program, 0, "outColor");
   glLinkProgram(gfx_env->text_program);
   glDetachShader(gfx_env->text_program, gfx_env->text_vert_shader);
   glDetachShader(gfx_env->text_program, gfx_env->text_geom_shader);
   glDetachShader(gfx_env->text_program, gfx_env->text_frag_shader);
   glUseProgram(0);
}

void eir_gfx_api_load_default_shaders(eir_gfx_env_t * gfx_env)
{
   if (!gfx_env)
   {
      return;
   }
   
   EIR_KER_LOG_MESSAGE("create default shaders");
   gfx_env->default_vert_shader = glCreateShader(GL_VERTEX_SHADER);
   eir_gfx_api_compile_shader(gfx_env->default_vert_shader, DEFAULT_VERTEX_SHADER_PATH);
   gfx_env->default_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
   eir_gfx_api_compile_shader(gfx_env->default_frag_shader, DEFAULT_FRAGMENT_SHADER_PATH);
   
   EIR_KER_LOG_MESSAGE("link program");
   gfx_env->default_program = glCreateProgram();
   glAttachShader(gfx_env->default_program, gfx_env->default_vert_shader);
   glAttachShader(gfx_env->default_program, gfx_env->default_frag_shader);
   glLinkProgram(gfx_env->default_program);
   glDetachShader(gfx_env->default_program, gfx_env->default_vert_shader);
   glDetachShader(gfx_env->default_program, gfx_env->default_frag_shader);
   glUseProgram(0);
}

void eir_gfx_api_compile_shader(eir_gfx_api_shader_handle_t shader, const char * filename)
{
   char * file_buffer = 0;

   EIR_KER_LOG_MESSAGE("read shade file %s", filename);
   
   if (!eir_sys_read_file(filename, false, &file_buffer))
   {
      EIR_KER_LOG_ERROR("cannot compile shader from file %s", filename);
      return;
   }

   EIR_KER_LOG_MESSAGE("compile shader source from buffer file");

   const char * shader_buffer = file_buffer;

   glShaderSource(shader, 1, &shader_buffer, NULL);
   glCompileShader(shader);

   GLint status;

   EIR_KER_LOG_MESSAGE("get shader status");

   glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
   if (!status)
   {
      char log[512];

      glGetShaderInfoLog(shader, 512, NULL, log);
      EIR_KER_LOG_ERROR("%s shader compilation failed: %s", filename, log);
   }
   EIR_SYS_FREE(file_buffer);
}

void eir_gfx_api_set_buffer_data(eir_gfx_sprite_batch_t * batch)
{
   glBufferData(
      GL_ARRAY_BUFFER,
      batch->sprites.used * sizeof (eir_gfx_sprite_t),
      batch->sprites.data,
      GL_DYNAMIC_DRAW
      );
}

void eir_gfx_api_build_sprite_batch(eir_gfx_env_t * gfx_env, eir_gfx_sprite_batch_t * batch)
{
   if (gfx_env && batch && batch->texture && batch->texture->image)
   {
      glUseProgram(gfx_env->sprite_program);

      EIR_KER_LOG_MESSAGE("create vao");
   
      glGenVertexArrays(1, &batch->vao);
      glBindVertexArray(batch->vao);

      EIR_KER_LOG_MESSAGE("create ibo and bind attributes");
   
      glGenBuffers(1, &batch->vbo);
      glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
      eir_gfx_api_set_buffer_data(batch);

      EIR_KER_LOG_MESSAGE("load and use atlas texture");

   //eir_gfx_image_t * image = eir_gfx_api_create_image(PLACE_HOLDER_IMAGE_PATH, true);

   //if (image)
   //{
   //   batch->texture[0] = eir_gfx_api_create_texture(image);
      glUniform1i(
	 glGetUniformLocation(gfx_env->sprite_program, "tex0"),
	 0
	 );
      glUniform2f(
	 glGetUniformLocation(gfx_env->sprite_program, "atlasSize"),
	 batch->texture->image->width,
	 batch->texture->image->height
	 );
   //eir_gfx_api_destroy_image(image);
      //}
      //else
      //{
      //EIR_KER_LOG_ERROR("cannot create texture from image file %d", PLACE_HOLDER_IMAGE_PATH);
      //}
      eir_gfx_api_bind_sprite_attributes(
	 glGetAttribLocation(gfx_env->sprite_program, "position"),
	 glGetAttribLocation(gfx_env->sprite_program, "size"),
	 glGetAttribLocation(gfx_env->sprite_program, "uv_offset"),
	 glGetAttribLocation(gfx_env->sprite_program, "uv_size"),
	 glGetAttribLocation(gfx_env->sprite_program, "color")
	 );
      glBindVertexArray(0);
      glUseProgram(0);
      batch->built = true;
      batch->modified = false;
      batch->program = gfx_env->sprite_program;
      eir_gfx_debug_log_sprite_batch(batch);
   }
}

void eir_gfx_api_build_text_batch(eir_gfx_env_t * gfx_env, eir_gfx_sprite_batch_t * batch)
{
   glUseProgram(gfx_env->text_program);

   EIR_KER_LOG_MESSAGE("create vao");
   glGenVertexArrays(1, &batch->vao);
   glBindVertexArray(batch->vao);

   EIR_KER_LOG_MESSAGE("create ibo and bind attributes");
   glGenBuffers(1, &batch->vbo);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   eir_gfx_api_set_buffer_data(batch);

   EIR_KER_LOG_MESSAGE("load and use atlas texture");

   eir_gfx_image_t * image = eir_gfx_api_create_image(DEFAULT_FONT_IMAGE_PATH, true);

   if (image)
   {
      batch->texture_id[0] = eir_gfx_api_create_texture(image);
      glUniform1i(glGetUniformLocation(gfx_env->text_program, "tex0"), 0);
      glUniform2f(glGetUniformLocation(gfx_env->text_program, "atlasSize"), image->width, image->height);
      eir_gfx_api_destroy_image(image);
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot create texture from image file %d", DEFAULT_FONT_IMAGE_PATH);
   }
   eir_gfx_api_bind_sprite_attributes(
      glGetAttribLocation(gfx_env->text_program, "position"),
      glGetAttribLocation(gfx_env->text_program, "size"),
      glGetAttribLocation(gfx_env->text_program, "uv_offset"),
      glGetAttribLocation(gfx_env->text_program, "uv_size"),
      glGetAttribLocation(gfx_env->text_program, "color")
      );
   glBindVertexArray(0);
   glUseProgram(0);
   batch->built = true;
   batch->modified = false;
   batch->program = gfx_env->text_program;
   eir_gfx_debug_log_sprite_batch(batch);
}

void eir_gfx_api_build_vertex_batch(eir_gfx_env_t * gfx_env, eir_gfx_vertex_batch_t * batch)
{
   if (!batch)
   {
      return;
   }

   glUseProgram(gfx_env->default_program);

   EIR_KER_LOG_MESSAGE("create vao");
   glGenVertexArrays(1, &batch->vao);
   glBindVertexArray(batch->vao);

   EIR_KER_LOG_MESSAGE("create ibo and bind attributes");
   glGenBuffers(1, &batch->vbo);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   glBufferData(
      GL_ARRAY_BUFFER,
      sizeof (eir_gfx_vertex_t) * batch->vertices.used,
      batch->vertices.data,
      GL_STATIC_DRAW
      );

   eir_gfx_api_att_handle_t pos_attr = glGetAttribLocation(
      gfx_env->default_program,
      "default_position"
      );
   eir_gfx_api_att_handle_t color_attr = glGetAttribLocation(
      gfx_env->default_program,
      "default_color"
      );

   glVertexAttribPointer(
      pos_attr,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_vertex_t),
      (void *)offsetof(eir_gfx_vertex_t, position)
      );
   glEnableVertexAttribArray(pos_attr);

   glVertexAttribPointer(
      color_attr,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_vertex_t),
      (void *)offsetof(eir_gfx_vertex_t, color)
      );
   glEnableVertexAttribArray(color_attr);

   glBindVertexArray(0);
   glUseProgram(0);
   batch->built = true;
   batch->modified = false;
   batch->program = gfx_env->default_program;
   eir_gfx_debug_log_vertex_batch(batch);
}

void eir_gfx_api_draw_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   glBindVertexArray(batch->vao);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   glDrawArrays(GL_POINTS, 0, batch->sprites.used);
}

void eir_gfx_api_draw_vertex_batch(eir_gfx_vertex_batch_t * batch)
{
   glBindVertexArray(batch->vao);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   glDrawArrays(
      batch->primitive_type == eir_gfx_primitive_type_lines ? GL_LINES : GL_TRIANGLE_STRIP,
      0,
      batch->vertices.used
      );
}

void eir_gfx_api_set_clear_color()
{
   glClearColor(1.f, .5f, 0.5f, 1.f);
}

void eir_gfx_api_clear_buffer()
{
   glClear(GL_COLOR_BUFFER_BIT);
}

void eir_gfx_api_bind_sprite_attributes(
   eir_gfx_api_att_handle_t pos_attr,
   eir_gfx_api_att_handle_t size_attr,
   eir_gfx_api_att_handle_t uv_offset_attr,
   eir_gfx_api_att_handle_t uv_size_attr,
   eir_gfx_api_att_handle_t color_attr
   )
{
   glVertexAttribPointer(
      pos_attr,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_sprite_t),
      (void *)offsetof(eir_gfx_sprite_t, position)
      );
   glEnableVertexAttribArray(pos_attr);

   glVertexAttribPointer(
      size_attr,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_sprite_t),
      (void *)offsetof(eir_gfx_sprite_t, size)
      );
   glEnableVertexAttribArray(size_attr);

   glVertexAttribPointer(
      uv_offset_attr,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_sprite_t),
      (void *)offsetof(eir_gfx_sprite_t, uv_offset)
      );
   glEnableVertexAttribArray(uv_offset_attr);
   
   glVertexAttribPointer(
      uv_size_attr,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_sprite_t),
      (void *)offsetof(eir_gfx_sprite_t, uv_size)
      );
   glEnableVertexAttribArray(uv_size_attr);

   glVertexAttribPointer(
      color_attr,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_sprite_t),
      (void *)offsetof(eir_gfx_sprite_t, color)
      );
   glEnableVertexAttribArray(color_attr);
}

void eir_gfx_api_release_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   glDeleteBuffers(1, &batch->vbo);
   glDeleteVertexArrays(1, &batch->vao);
   glDeleteTextures(1, batch->texture_id);
   EIR_KER_RELEASE_ARRAY(batch->sprites);
}

void eir_gfx_api_release_vertex_batch(eir_gfx_vertex_batch_t * batch)
{
   glDeleteBuffers(1, &batch->vbo);
   EIR_KER_RELEASE_ARRAY(batch->vertices);
}

void eir_gfx_api_unload_sprite_shaders(eir_gfx_env_t * gfx_env)
{
   if(!gfx_env)
   {
      return;
   }
   glDeleteProgram(gfx_env->sprite_program);
   glDeleteShader(gfx_env->sprite_frag_shader);
   glDeleteShader(gfx_env->sprite_geom_shader);
   glDeleteShader(gfx_env->sprite_vert_shader);
}

void eir_gfx_api_unload_text_shaders(eir_gfx_env_t * gfx_env)
{
   if(!gfx_env)
   {
      return;
   }
   glDeleteProgram(gfx_env->text_program);
   glDeleteShader(gfx_env->text_frag_shader);
   glDeleteShader(gfx_env->text_geom_shader);
   glDeleteShader(gfx_env->text_vert_shader);
}

void eir_gfx_api_unload_default_shaders(eir_gfx_env_t * gfx_env)
{
   if(!gfx_env)
   {
      return;
   }
   glDeleteProgram(gfx_env->default_program);
   glDeleteShader(gfx_env->default_frag_shader);
   glDeleteShader(gfx_env->default_vert_shader);
}
