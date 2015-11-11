#include "eir_gfx_api_func.h"
#include "../system/eir_file_system.h"
#include "../kernel/eir_log.h"
#include "eir_image_api_func.h"
#include "eir_gfx_defines.h"

static void eir_gfx_debug_log_batch(eir_gfx_sprite_batch_t * batch)
{
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
   EIR_KER_LOG_MESSAGE("%s", "batch info:");
   EIR_KER_LOG_MESSAGE("vbo: %d", batch->vbo);
   EIR_KER_LOG_MESSAGE("vao: %d", batch->vao);
   EIR_KER_LOG_MESSAGE("texture: %d", batch->texture[0]);
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
}

void eir_gfx_api_init()
{
   EIR_KER_LOG_MESSAGE("init opengl");
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
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

void eir_gfx_api_load_default_shaders(eir_gfx_env_t * gfx_env)
{
   if (!gfx_env)
   {
      return;
   }

   /* create shaders */
   
   EIR_KER_LOG_MESSAGE("create shaders");

   gfx_env->sprite_vert_shader = glCreateShader(GL_VERTEX_SHADER);
   eir_gfx_api_compile_shader(gfx_env->sprite_vert_shader, VERTEX_SHADER_PATH);
   gfx_env->sprite_geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
   eir_gfx_api_compile_shader(gfx_env->sprite_geom_shader, GEOMETRY_SHADER_PATH);
   gfx_env->sprite_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
   eir_gfx_api_compile_shader(gfx_env->sprite_frag_shader, FRAGMENT_SHADER_PATH);
   
   /* link program */

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
}

void eir_gfx_api_build_sprite_batch(eir_gfx_env_t * gfx_env, eir_gfx_sprite_batch_t * batch)
{
   glUseProgram(gfx_env->sprite_program);

   /* create vao */

   EIR_KER_LOG_MESSAGE("create vao");

   glGenVertexArrays(1, &batch->vao);
   glBindVertexArray(batch->vao);

   /* make ibo and bind ibo attributes for sprites */

   EIR_KER_LOG_MESSAGE("create ibo and bind attributes");

   glGenBuffers(1, &batch->vbo);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   eir_gfx_api_set_buffer_data(batch);

   /* load and use atlas texture */

   EIR_KER_LOG_MESSAGE("load and use atlas texture");

   glGenTextures(1,batch->texture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, batch->texture[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   eir_gfx_image_t * image = eir_gfx_api_create_image(PLACE_HOLDER_IMAGE_PATH, true);

   if (image)
   {
      EIR_KER_LOG_MESSAGE("create texture image: w=%d, h=%d", image->width, image->height);
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
      glUniform1i(glGetUniformLocation(gfx_env->sprite_program, "tex0"), 0);
      glUniform2f(glGetUniformLocation(gfx_env->sprite_program, "atlasSize"), image->width, image->height);
      eir_gfx_api_destroy_image(image);
   }

   /* bind attributes */

   eir_gfx_api_bind_sprite_attributes(
      glGetAttribLocation(gfx_env->sprite_program, "position"),
      glGetAttribLocation(gfx_env->sprite_program, "size"),
      glGetAttribLocation(gfx_env->sprite_program, "uv_offset"),
      glGetAttribLocation(gfx_env->sprite_program, "uv_size"),
      glGetAttribLocation(gfx_env->sprite_program, "color")
      );

   /* clear all */

   glBindVertexArray(0);
   glUseProgram(0);
   batch->built = true;
   batch->modified = false;
   eir_gfx_debug_log_batch(batch);
}

void eir_gfx_api_build_text_batch(eir_gfx_env_t * gfx_env, eir_gfx_sprite_batch_t * batch)
{
   glUseProgram(gfx_env->sprite_program);

   /* create vao */

   EIR_KER_LOG_MESSAGE("create vao");

   glGenVertexArrays(1, &batch->vao);
   glBindVertexArray(batch->vao);

   /* make ibo and bind ibo attributes for sprites */

   EIR_KER_LOG_MESSAGE("create ibo and bind attributes");

   glGenBuffers(1, &batch->vbo);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   eir_gfx_api_set_buffer_data(batch);

   /* load and use atlas texture */

   EIR_KER_LOG_MESSAGE("load and use atlas texture");

   glGenTextures(1,batch->texture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, batch->texture[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   eir_gfx_image_t * image = eir_gfx_api_create_image(DEFAULT_FONT_IMAGE_PATH, true);

   if (image)
   {
      EIR_KER_LOG_MESSAGE("create texture image: w=%d, h=%d", image->width, image->height);
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
      glUniform1i(glGetUniformLocation(gfx_env->sprite_program, "tex0"), 0);
      glUniform2f(glGetUniformLocation(gfx_env->sprite_program, "atlasSize"), image->width, image->height);
      eir_gfx_api_destroy_image(image);
   }
   else
   {
      EIR_KER_LOG_ERROR("cannot create texture from image file %d", DEFAULT_FONT_IMAGE_PATH);
   }
   
   /* bind attributes */

   eir_gfx_api_bind_sprite_attributes(
      glGetAttribLocation(gfx_env->sprite_program, "position"),
      glGetAttribLocation(gfx_env->sprite_program, "size"),
      glGetAttribLocation(gfx_env->sprite_program, "uv_offset"),
      glGetAttribLocation(gfx_env->sprite_program, "uv_size"),
      glGetAttribLocation(gfx_env->sprite_program, "color")
      );

   /* clear all */

   glBindVertexArray(0);
   glUseProgram(0);
   batch->built = true;
   batch->modified = false;
   eir_gfx_debug_log_batch(batch);
}

void eir_gfx_api_draw_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   glBindVertexArray(batch->vao);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   glDrawArrays(GL_POINTS, 0, batch->sprites.used);
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

void eir_gfx_api_release_batch(eir_gfx_sprite_batch_t * batch)
{
   glDeleteBuffers(1, &batch->vbo);
   glDeleteVertexArrays(1, &batch->vao);
   glDeleteTextures(1, batch->texture);
   EIR_KER_RELEASE_ARRAY(batch->sprites);
}

void eir_gfx_api_unload_default_shaders(eir_gfx_env_t * gfx_env)
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
