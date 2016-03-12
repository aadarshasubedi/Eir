#include "eir_gfx_api_func.h"
#include "../system/eir_sys_file_system.h"
#include "../kernel/eir_ker_log.h"
#include "eir_gfx_image_api_func.h"
#include "eir_gfx_defines.h"

static void eir_gfx_debug_log_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
   EIR_KER_LOG_MESSAGE("%s", "sprite batch info:");
   EIR_KER_LOG_MESSAGE("vbo: %d", batch->vbo);
   EIR_KER_LOG_MESSAGE("vao: %d", batch->vao);
   EIR_KER_LOG_MESSAGE("texture: %d", batch->texture->id);
   EIR_KER_LOG_MESSAGE("sprites count: %d", batch->sprites.used);
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
}

static void eir_gfx_debug_log_rect_batch(eir_gfx_rect_batch_t * batch)
{
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
   EIR_KER_LOG_MESSAGE("%s", "rect batch info:");
   EIR_KER_LOG_MESSAGE("vbo: %d", batch->vbo);
   EIR_KER_LOG_MESSAGE("vao: %d", batch->vao);
   EIR_KER_LOG_MESSAGE("rect count: %d", batch->rects.used);
   EIR_KER_LOG_MESSAGE("%s", "---------------------------");
}

void eir_gfx_api_init()
{
   EIR_KER_LOG_MESSAGE("init opengl");
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);
}

void eir_gfx_api_release()
{
}

eir_gfx_api_texture_handle_t eir_gfx_api_create_texture(const eir_gfx_image_t * image)
{
   eir_gfx_api_texture_handle_t texture = EIR_GFX_INVALID_TEXTURE_HANDLE;

   if (image)
   {
      glGenTextures(1, &texture);
      EIR_KER_LOG_MESSAGE(
       "create texture image: w=%d, h=%d, id = %d",
       image->width,
       image->height,
       texture
       );
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

void eir_gfx_api_bind_texture(eir_gfx_api_texture_handle_t texture_handle)
{
   glBindTexture(GL_TEXTURE_2D, texture_handle);
}

void eir_gfx_api_bind_vertex_array(eir_gfx_api_vao_handle_t vao_handle)
{
   glBindVertexArray(vao_handle);
}

void eir_gfx_api_bind_array_buffer(eir_gfx_api_vbo_handle_t vbo_handle)
{
   glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
}

void eir_gfx_api_unbind_vertex_array()
{
   glBindVertexArray(0);
}

void eir_gfx_api_use_program(eir_gfx_api_program_handle_t program_handle)
{
   glUseProgram(program_handle);
}

void eir_gfx_api_unuse_program()
{
   glUseProgram(0);
}

void eir_gfx_api_set_program_mat4(const char * var_name, eir_gfx_api_program_handle_t program_handle, const eir_mth_mat4_t * m)
{
   glUniformMatrix4fv(glGetUniformLocation(program_handle, var_name), 1, GL_FALSE, &m->values[0][0]);
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

void eir_gfx_api_load_rect_shaders(eir_gfx_env_t * gfx_env)
{
   if (!gfx_env)
   {
      return;
   }
   
   EIR_KER_LOG_MESSAGE("create rect shaders");
   gfx_env->rect_vert_shader = glCreateShader(GL_VERTEX_SHADER);
   eir_gfx_api_compile_shader(gfx_env->rect_vert_shader, RECT_VERTEX_SHADER_PATH);
   gfx_env->rect_geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
   eir_gfx_api_compile_shader(gfx_env->rect_geom_shader, RECT_GEOMETRY_SHADER_PATH);
   gfx_env->rect_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
   eir_gfx_api_compile_shader(gfx_env->rect_frag_shader, RECT_FRAGMENT_SHADER_PATH);
   
   EIR_KER_LOG_MESSAGE("link program");
   gfx_env->rect_program = glCreateProgram();
   glAttachShader(gfx_env->rect_program, gfx_env->rect_vert_shader);
   glAttachShader(gfx_env->rect_program, gfx_env->rect_geom_shader);
   glAttachShader(gfx_env->rect_program, gfx_env->rect_frag_shader);
   glLinkProgram(gfx_env->rect_program);
   glDetachShader(gfx_env->rect_program, gfx_env->rect_vert_shader);
   glDetachShader(gfx_env->rect_program, gfx_env->rect_geom_shader);
   glDetachShader(gfx_env->rect_program, gfx_env->rect_frag_shader);
   glUseProgram(0);
}

void eir_gfx_api_compile_shader(
   eir_gfx_api_shader_handle_t shader,
   const char * filename
   )
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

void eir_gfx_api_set_sprite_buffer_data(eir_gfx_sprite_batch_t * batch)
{
   glBufferData(
      GL_ARRAY_BUFFER,
      batch->sprites.used * sizeof (eir_gfx_sprite_t),
      batch->sprites.data,
      GL_DYNAMIC_DRAW
      );
}

void eir_gfx_api_set_rect_buffer_data(eir_gfx_rect_batch_t * batch)
{
   glBufferData(
      GL_ARRAY_BUFFER,
      batch->rects.used * sizeof (eir_gfx_rect_t),
      batch->rects.data,
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
      eir_gfx_api_set_sprite_buffer_data(batch);

      EIR_KER_LOG_MESSAGE("load and use atlas texture");

      glUniform1i(
       glGetUniformLocation(gfx_env->sprite_program, "tex0"),
       0
       );
      glUniform2f(
       glGetUniformLocation(gfx_env->sprite_program, "atlasSize"),
       batch->texture->image->width,
       batch->texture->image->height
       );
      eir_gfx_api_bind_sprite_attributes(
       glGetAttribLocation(gfx_env->sprite_program, "position"),
       glGetAttribLocation(gfx_env->sprite_program, "size"),
       glGetAttribLocation(gfx_env->sprite_program, "uv_offset"),
       glGetAttribLocation(gfx_env->sprite_program, "uv_size"),
       glGetAttribLocation(gfx_env->sprite_program, "color")
       );
      glBindVertexArray(0);
      glUseProgram(0);
      batch->info |= eir_gfx_item_flag_built;
      batch->info &= ~eir_gfx_item_flag_modified;
      batch->program = gfx_env->sprite_program;
      eir_gfx_debug_log_sprite_batch(batch);
   }
}

void eir_gfx_api_build_text_batch(eir_gfx_env_t * gfx_env, eir_gfx_sprite_batch_t * batch)
{
   if (batch && gfx_env)
   {
      glUseProgram(gfx_env->text_program);

      EIR_KER_LOG_MESSAGE("create vao");
      glGenVertexArrays(1, &batch->vao);
      glBindVertexArray(batch->vao);

      EIR_KER_LOG_MESSAGE("create ibo and bind attributes");
      glGenBuffers(1, &batch->vbo);
      glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
      eir_gfx_api_set_sprite_buffer_data(batch);

      EIR_KER_LOG_MESSAGE("load and use atlas texture");

      bool result = eir_gfx_api_load_image(
       DEFAULT_FONT_IMAGE_PATH,
       false,
       &gfx_env->text_image
       );

      if (result)
      {
       gfx_env->text_texture.image = &gfx_env->text_image;
       gfx_env->text_texture.id = eir_gfx_api_create_texture(&gfx_env->text_image);
       batch->texture = &gfx_env->text_texture;
       glUniform1i(glGetUniformLocation(gfx_env->text_program, "tex0"), 0);
       glUniform2f(
          glGetUniformLocation(gfx_env->text_program, "atlasSize"),
          gfx_env->text_image.width,
          gfx_env->text_image.height
          );
    }
    else
    {
       EIR_KER_LOG_ERROR(
          "cannot create texture from image file %d",
          DEFAULT_FONT_IMAGE_PATH
          );
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
    batch->info |= eir_gfx_item_flag_built;
    batch->info &= ~eir_gfx_item_flag_modified;
    batch->program = gfx_env->text_program;
    eir_gfx_debug_log_sprite_batch(batch);
 }
}

void eir_gfx_api_build_rect_batch(eir_gfx_env_t * gfx_env, eir_gfx_rect_batch_t * batch)
{
   if (batch && gfx_env)
   {
      glUseProgram(gfx_env->rect_program);

      EIR_KER_LOG_MESSAGE("create vao");
      
      glGenVertexArrays(1, &batch->vao);
      glBindVertexArray(batch->vao);

      EIR_KER_LOG_MESSAGE("create ibo and bind attributes");
      
      glGenBuffers(1, &batch->vbo);
      glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
      eir_gfx_api_set_rect_buffer_data(batch);

      eir_gfx_api_bind_rect_attributes(
       glGetAttribLocation(gfx_env->rect_program, "rect_position"),
       glGetAttribLocation(gfx_env->rect_program, "rect_size"),
       glGetAttribLocation(gfx_env->rect_program, "rect_color")
       );
      glBindVertexArray(0);
      glUseProgram(0);
      batch->info |= eir_gfx_item_flag_built;
      batch->info &= ~eir_gfx_item_flag_modified;
      batch->program = gfx_env->rect_program;
      eir_gfx_debug_log_rect_batch(batch);
   }
}

void eir_gfx_api_draw_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   glBindVertexArray(batch->vao);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   glDrawArrays(GL_POINTS, 0, batch->sprites.used);
}

void eir_gfx_api_draw_rect_batch(eir_gfx_rect_batch_t * batch)
{
   glBindVertexArray(batch->vao);
   glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
   glDrawArrays(GL_POINTS, 0, batch->rects.used);
}

void eir_gfx_api_set_clear_color()
{
   glClearColor(0.f, 0.f, 0.f, 1.f);
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

void eir_gfx_api_bind_rect_attributes(
   eir_gfx_api_att_handle_t pos_attr,
   eir_gfx_api_att_handle_t size_attr,
   eir_gfx_api_att_handle_t color_attr
   )
{
   glVertexAttribPointer(
      pos_attr,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_rect_t),
      (void *)offsetof(eir_gfx_rect_t, position)
      );
   glEnableVertexAttribArray(pos_attr);

   glVertexAttribPointer(
      size_attr,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_rect_t),
      (void *)offsetof(eir_gfx_rect_t, size)
      );
   glEnableVertexAttribArray(size_attr);

   glVertexAttribPointer(
      color_attr,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof (eir_gfx_rect_t),
      (void *)offsetof(eir_gfx_rect_t, color)
      );
   glEnableVertexAttribArray(color_attr);
}

void eir_gfx_api_destroy_texture(eir_gfx_api_texture_handle_t texture_handle)
{
   glDeleteTextures(1, &texture_handle);
}

void eir_gfx_api_release_sprite_batch(eir_gfx_sprite_batch_t * batch)
{
   glDeleteBuffers(1, &batch->vbo);
   glDeleteVertexArrays(1, &batch->vao);
   EIR_KER_RELEASE_ARRAY(batch->sprites);
}

void eir_gfx_api_release_rect_batch(eir_gfx_rect_batch_t * batch)
{
   glDeleteBuffers(1, &batch->vbo);
   EIR_KER_RELEASE_ARRAY(batch->rects);
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

void eir_gfx_api_unload_rect_shaders(eir_gfx_env_t * gfx_env)
{
   if(!gfx_env)
   {
      return;
   }
   glDeleteProgram(gfx_env->rect_program);
   glDeleteShader(gfx_env->rect_frag_shader);
   glDeleteShader(gfx_env->rect_vert_shader);
}

void eir_gfx_api_check_for_error()
{
   GLenum error_code = glGetError();

   while (error_code != GL_NO_ERROR)
   {
      if (GL_INVALID_OPERATION == error_code)
      {
       EIR_KER_LOG_ERROR("invalid operation");
    }
    else if (GL_INVALID_ENUM == error_code)
    {
       EIR_KER_LOG_ERROR("invalid enum");
    }
    else if (GL_INVALID_VALUE == error_code)
    {
       EIR_KER_LOG_ERROR("invalid value");
    }
    else if (GL_OUT_OF_MEMORY == error_code)
    {
       EIR_KER_LOG_ERROR("out of memory");
    }
    else if (GL_INVALID_FRAMEBUFFER_OPERATION == error_code)
    {
       EIR_KER_LOG_ERROR("invalid frame buffer");
    }
    else
    {
       EIR_KER_LOG_ERROR("unknown error code %d", error_code);
    }
    error_code = glGetError();
 }
}

