// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string.h>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <time.h>
#include "SOIL.h"

#include "Object.h"
#include "Shader.h"
#include "Camera.h"

#include "vec.hpp"
#include "mat.hpp"
#include "transform.hpp"

void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);
void create_GLU_menus();
void main_menus(int values);
void make_scene_txt();
void Load();
void Save();
void Jump();

typedef struct _Scene{
  std::string obj_name;
  float angle;
  float scale;
  kmuvcl::math::vec3f translate;
  std::string texture_name;
  kmuvcl::math::vec3f up;
  kmuvcl::math::vec3f front;
}Scene;

GLuint program;

GLint  loc_a_vertex;
GLint  loc_a_normal;

GLint  loc_u_pvm_matrix;
GLint  loc_u_view_matrix;
GLint  loc_u_model_matrix;
GLint  loc_u_normal_matrix;

GLint  loc_u_light_vector;

GLint  loc_u_light_ambient;
GLint  loc_u_light_diffuse;
GLint  loc_u_light_specular;

GLint  loc_u_material_ambient;
GLint  loc_u_material_diffuse;
GLint  loc_u_material_specular;
GLint  loc_u_material_shininess;

kmuvcl::math::mat4x4f   mat_PVM;
GLint     loc_a_texcoord;
GLint     loc_u_texid;

kmuvcl::math::vec4f light_vector      = kmuvcl::math::vec4f(10.0f, -5.0f, 10.0f);
kmuvcl::math::vec4f light_ambient     = kmuvcl::math::vec4f(3.0f, 3.0f, 3.0f, 3.0f);
kmuvcl::math::vec4f light_diffuse     = kmuvcl::math::vec4f(3.0f, 3.0f, 3.0f, 3.0f);
kmuvcl::math::vec4f light_specular    = kmuvcl::math::vec4f(5.0f, 5.0f, 5.0f, 5.0f);

kmuvcl::math::vec4f material_ambient  = kmuvcl::math::vec4f(0.464f, 0.393f, 0.094f, 1.0f);
kmuvcl::math::vec4f material_diffuse  = kmuvcl::math::vec4f(2.464f, 2.393f, 2.094f, 1.0f);
kmuvcl::math::vec4f material_specular = kmuvcl::math::vec4f(5.0f, 5.0f, 5.0f, 10.0f);
float     material_shininess= 50.0f;

std::string g_filename[11];
Object      g_dear, g_ground, g_giraffe, g_ground2, g_rabbit2, g_human, g_cat,g_statue,g_rabbit, g_ground3, g_ground4;        // object
Camera		  g_camera;	      // Camera
Scene dear, ground, giraffe, ground2, rabbit2, human, cat,statue,rabbit, ground3, ground4; // Scene

float rotating = 0.0f, rotate_v = 3.0f;
float rotate = 2.0f, fast = 0.1f;
int m = 1, pm = 0;
int jump = 0, jump_end = 0;
int mode = 0;
int j_u = 10, j_d = 0;
int t_cnt = 10, cnt = 1;

std::chrono::time_point<std::chrono::system_clock> prev, curr;

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK)
  {
      std::cerr << "failed to initialize glew" << std::endl;
      return -1;
  }
  create_GLU_menus();
  init();
  glutMainLoop();

  return 0;
}

void init()
{
  kmuvcl::math::vec3f up(0,1,0); kmuvcl::math::vec3f front(0,0,1);
  // Obj 추가할 때 마다 여기에 추가/////////////////////////////////////////
  kmuvcl::math::vec3f d(-2.3f, 0.0f, -8.0f);
  kmuvcl::math::vec3f f(-3.0f, -0.5f, -13.0f);
  kmuvcl::math::vec3f s(2.5f, 0.0f, -16.0f);
  kmuvcl::math::vec3f t(0.0f, -0.5f, -13.0f);
  kmuvcl::math::vec3f p(1.5f, 0.0f, -10.0f);
  kmuvcl::math::vec3f h(-1.3f, 0.0f, -8.0f);
  kmuvcl::math::vec3f c(0.2f, 0.0f, -7.0f);
  kmuvcl::math::vec3f a(-0.3f, 0.0f, -6.0f);
  kmuvcl::math::vec3f r(2.0f, 0.0f, -9.0f);
  kmuvcl::math::vec3f g3(0.0f, -0.5f, -13.0f);
  kmuvcl::math::vec3f g4(0.0f, -0.5f, -13.0f);

  g_filename[0] = "./data/Deer.obj";
  g_filename[1] = "./data/Grass_02.obj";
  g_filename[2] = "./data/Giraffe.obj";
  g_filename[3] = "./data/Grass_02.obj";
  g_filename[4] = "./data/Rabbit.obj";
  g_filename[5] = "./data/human.obj";
  g_filename[6] = "./data/cat.obj";
  g_filename[7] = "./data/statue.obj";
  g_filename[8] = "./data/Rabbit.obj";
  g_filename[9] = "./data/Grass_02.obj";
  g_filename[10] = "./data/Grass_02.obj";

  g_dear.load_simple_obj(g_filename[0]);
  g_ground.load_simple_obj(g_filename[1]);
  g_giraffe.load_simple_obj(g_filename[2]);
  g_ground2.load_simple_obj(g_filename[3]);
  g_rabbit2.load_simple_obj(g_filename[4]);
  g_human.load_simple_obj(g_filename[5]);
  g_cat.load_simple_obj(g_filename[6]);
  g_statue.load_simple_obj(g_filename[7]);
  g_rabbit.load_simple_obj(g_filename[8]);
  g_ground3.load_simple_obj(g_filename[9]);
  g_ground4.load_simple_obj(g_filename[10]);

  dear.obj_name    = g_filename[0]; dear.angle     = 0.0f;   dear.scale   = 0.5f;  dear.translate  = d; dear.texture_name  = "Deer_body_D.tga"; dear.up = up; dear.front = front;
  ground.obj_name  = g_filename[1]; ground.angle   = 0.0f;   ground.scale    = 0.25f;  ground.translate   = f; ground.texture_name   = "grass_diff.tga"; ground.up = up; ground.front = front;
  giraffe.obj_name = g_filename[2]; giraffe.angle  = 70.0f;  giraffe.scale  = 0.7f;  giraffe.translate = s; giraffe.texture_name = "null";giraffe.up = up; giraffe.front = front;
  ground2.obj_name = g_filename[3]; ground2.angle  = 0.0f;   ground2.scale     = 1.0f;  ground2.translate    = t; ground2.texture_name    = "grass_diff.tga";   ground2.up = up; ground2.front = front;
  rabbit2.obj_name = g_filename[4]; rabbit2.angle  = 0.0f;   rabbit2.scale  = 0.8f; rabbit2.translate = p; rabbit2.texture_name = "Rabbit_D.tga";  rabbit2.up = up; rabbit2.front = front;
  human.obj_name   = g_filename[5]; human.angle    = 300.0f; human.scale  = 0.7f;  human.translate = h; human.texture_name = "null"; human.up = up; human.front = front;
  cat.obj_name     = g_filename[6]; cat.angle      = 15.0f;  cat.scale    = 1.0f;  cat.translate   = c ;cat.texture_name = "cat_diff.tga";  cat.up = up; cat.front = front;
  statue.obj_name  = g_filename[7]; statue.angle   = 15.0f;  statue.scale = 0.5f;  statue.translate   = a ;statue.texture_name = "statue_d.dds"; statue.up = up; statue.front = front;
  rabbit.obj_name  = g_filename[8]; rabbit.angle   = -15.0f; rabbit.scale = 0.8f;  rabbit.translate   = r ;rabbit.texture_name = "Rabbit_D.tga"; rabbit.up = up; rabbit.front = front;
  ground3.obj_name = g_filename[9]; ground3.angle  = 0.0f;   ground3.scale = 0.25f;  ground3.translate = g3; ground3.texture_name = "grass_diff.tga"; ground3.up = up; ground3.front = front;
  ground4.obj_name = g_filename[10]; ground4.angle = 0.0f;   ground4.scale = 0.25f;  ground4.translate = g3; ground4.texture_name = "grass_diff.tga"; ground4.up = up; ground4.front = front;

  kmuvcl::math::mat4x4f mat_rotate = kmuvcl::math::rotate(rabbit.angle, 0.0f, 1.0f, 0.0f);
  kmuvcl::math::vec4f r_f(rabbit.front(0), rabbit.front(1), rabbit.front(2), 0);
  r_f = mat_rotate * r_f;
  rabbit.front = kmuvcl::math::vec3f(r_f(0), r_f(1), r_f(2));
  kmuvcl::math::mat4x4f mat_rotate2 = kmuvcl::math::rotate(rabbit2.angle, 0.0f, 1.0f, 0.0f);
  kmuvcl::math::vec4f r_f2(rabbit2.front(0), rabbit2.front(1), rabbit2.front(2), 0);
  r_f2 = mat_rotate2 * r_f2;
  rabbit2.front = kmuvcl::math::vec3f(r_f2(0), r_f2(1), r_f2(2));
  ////////////////////////////////////////////////////////////////////
  //g_model.print();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for filled polygon rendering

  glEnable(GL_DEPTH_TEST);

  program = Shader::create_program("./shader/phong_vert.glsl", "./shader/phong_frag.glsl");
  //Model
  loc_u_pvm_matrix         = glGetUniformLocation(program, "u_pvm_matrix");
  loc_u_view_matrix        = glGetUniformLocation(program, "u_view_matrix");
  loc_u_model_matrix       = glGetUniformLocation(program, "u_model_matrix");
  loc_u_normal_matrix      = glGetUniformLocation(program, "u_normal_matrix");
  //Light
  loc_u_light_vector       = glGetUniformLocation(program, "u_light_vector");
  loc_u_light_ambient      = glGetUniformLocation(program, "u_light_ambient");
  loc_u_light_diffuse      = glGetUniformLocation(program, "u_light_diffuse");
  loc_u_light_specular     = glGetUniformLocation(program, "u_light_specular");
  //Material
  loc_u_material_ambient   = glGetUniformLocation(program, "u_material_ambient");
  loc_u_material_diffuse   = glGetUniformLocation(program, "u_material_diffuse");
  loc_u_material_specular  = glGetUniformLocation(program, "u_material_specular");
  loc_u_material_shininess = glGetUniformLocation(program, "u_material_shininess");
  //Vertex
  loc_a_vertex             = glGetAttribLocation(program, "a_vertex");
  loc_a_normal             = glGetAttribLocation(program, "a_normal");

  loc_a_texcoord 			     = glGetAttribLocation(program, "a_texcoord");
  loc_u_texid              = glGetUniformLocation(program, "u_texid");

  prev = curr = std::chrono::system_clock::now();
}

void display()
{
  if (m == pm) return;
  m = pm;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  // Camera setting
  kmuvcl::math::mat4x4f   mat_Proj, mat_View_inv, mat_Model, mat_trans, mat_scale, mat_rotate;
  kmuvcl::math::mat3x3f   mat_Normal;

  // camera intrinsic param
  mat_Proj = kmuvcl::math::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);

  // camera extrinsic param
  mat_View_inv = kmuvcl::math::lookAt(
    g_camera.position()(0), g_camera.position()(1), g_camera.position()(2),				// eye position
		g_camera.center_position()(0), g_camera.center_position()(1), g_camera.center_position()(2), // center position
		g_camera.up_direction()(0), g_camera.up_direction()(1), g_camera.up_direction()(2)			// up direction
    );

  kmuvcl::math::mat4x4f mat_View = kmuvcl::math::inverse(mat_View_inv);
  glUniformMatrix4fv(loc_u_view_matrix, 1, false, mat_View);

  glUniform3fv(loc_u_light_vector, 1, light_vector);
  glUniform4fv(loc_u_light_ambient, 1, light_ambient);
  glUniform4fv(loc_u_light_diffuse, 1, light_diffuse);
  glUniform4fv(loc_u_light_specular, 1, light_specular);

  glUniform4fv(loc_u_material_ambient, 1, material_ambient);
  glUniform4fv(loc_u_material_diffuse, 1, material_diffuse);
  glUniform4fv(loc_u_material_specular, 1, material_specular);
  glUniform1f(loc_u_material_shininess, material_shininess);

  Shader::check_gl_error("glUniform4fv");

  //dear 그려버리기~/////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(dear.scale, dear.scale, dear.scale);
  mat_rotate = kmuvcl::math::rotate(dear.angle, 0.0f, 0.6f, 0.0f);
  mat_trans = kmuvcl::math::translate(dear.translate(0), dear.translate(1), dear.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_dear.V[i] = mat_Model * g_dear.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_dear.textureid);
  glUniform1i(loc_u_texid, 0);

  g_dear.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //ground 그려버리기~//////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(-3.0f, 0.1f, 5.0f);
  mat_rotate = kmuvcl::math::rotate(ground.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(ground.translate(0), ground.translate(1), ground.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_ground.V[i] = mat_Model * g_ground.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_ground.textureid);
  glUniform1i(loc_u_texid, 0);

  g_ground.draw(loc_a_vertex, loc_a_normal,loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //giraffe 그려버리기~////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(giraffe.scale, giraffe.scale, giraffe.scale);
  mat_rotate = kmuvcl::math::rotate(giraffe.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(giraffe.translate(0), giraffe.translate(1), giraffe.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_giraffe.V[i] = mat_Model * g_giraffe.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_giraffe.textureid);
  glUniform1i(loc_u_texid, 0);

  g_giraffe.draw(loc_a_vertex, loc_a_normal,loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //ground2 그려버리기~/////////////////////////////////////
  mat_scale = kmuvcl::math::scale(0.7f, 0.1f, 5.0f);
  mat_rotate = kmuvcl::math::rotate(ground2.angle, 0.0f, 0.6f, 0.0f);
  mat_trans = kmuvcl::math::translate(ground2.translate(0), ground2.translate(1), ground2.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_ground2.V[i] = mat_Model * g_ground2.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_ground2.textureid);
  glUniform1i(loc_u_texid, 0);

  g_ground2.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //rabbit2 그려버리기~/////////////////////////////////////
  mat_scale = kmuvcl::math::scale(rabbit2.scale, rabbit2.scale, rabbit2.scale);
  mat_rotate = kmuvcl::math::rotate(rabbit2.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(rabbit2.translate(0), rabbit2.translate(1), rabbit2.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  //for(int i=0; i<8; i++) //boundary box도 물체이동
  //  g_dear.V[i] = mat_Model * g_dear.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_rabbit2.textureid);
  glUniform1i(loc_u_texid, 0);

  g_rabbit2.draw(loc_a_vertex, loc_a_normal, loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //human 그려버리기~///////////////////////////////////////
  mat_scale = kmuvcl::math::scale(human.scale, human.scale, human.scale);
  mat_rotate = kmuvcl::math::rotate(human.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(human.translate(0), human.translate(1), human.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_human.V[i] = mat_Model * g_human.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  g_human.draw(loc_a_vertex, loc_a_normal,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //cat 그려버리기~/////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(cat.scale, cat.scale, cat.scale);
  mat_rotate = kmuvcl::math::rotate(cat.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(cat.translate(0), cat.translate(1), cat.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_cat.V[i] = mat_Model * g_cat.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_cat.textureid);
  glUniform1i(loc_u_texid, 0);

  g_cat.draw(loc_a_vertex, loc_a_normal,loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");
  ///////////////////////////////////////////////////////////

  //statue 그려버리기~////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(statue.scale, statue.scale, statue.scale);
  mat_rotate = kmuvcl::math::rotate(statue.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(statue.translate(0), statue.translate(1), statue.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_giraffe.V[i] = mat_Model * g_giraffe.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_statue.textureid);
  glUniform1i(loc_u_texid, 0);

  g_statue.draw(loc_a_vertex, loc_a_normal,loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //rabit////////////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(rabbit.scale, rabbit.scale, rabbit.scale);
  mat_rotate = kmuvcl::math::rotate(rabbit.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(rabbit.translate(0), rabbit.translate(1), rabbit.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_giraffe.V[i] = mat_Model * g_giraffe.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_rabbit.textureid);
  glUniform1i(loc_u_texid, 0);

  g_rabbit.draw(loc_a_vertex, loc_a_normal,loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");
  //ground3 그려버리기~//////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(-3.0f, 0.1f, 5.0f);
  mat_rotate = kmuvcl::math::rotate(ground3.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(ground3.translate(0), ground3.translate(1), ground3.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_ground.V[i] = mat_Model * g_ground.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_ground3.textureid);
  glUniform1i(loc_u_texid, 0);

  g_ground3.draw(loc_a_vertex, loc_a_normal,loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");

  //ground 4그려버리기~//////////////////////////////////////////
  mat_scale = kmuvcl::math::scale(-3.0f, 0.1f, 5.0f);
  mat_rotate = kmuvcl::math::rotate(ground4.angle, 0.0f, 1.0f, 0.0f);
  mat_trans = kmuvcl::math::translate(ground4.translate(0), ground4.translate(1), ground4.translate(2));
  mat_Model = mat_trans * mat_scale * mat_rotate;
  mat_PVM = mat_Proj*mat_View_inv*mat_Model;

  for(int i=0; i<8; i++) //boundary box도 물체이동
    g_ground.V[i] = mat_Model * g_ground.V[i];

  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      mat_Normal(i,j) = mat_Model(i,j);

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_model_matrix, 1, false, mat_Model);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);

  glBindTexture(GL_TEXTURE_2D, g_ground4.textureid);
  glUniform1i(loc_u_texid, 0);

  g_ground4.draw(loc_a_vertex, loc_a_normal,loc_a_texcoord,
    loc_u_material_ambient, loc_u_material_diffuse,
    loc_u_material_specular, loc_u_material_shininess);
  Shader::check_gl_error("draw");


  glUseProgram(0);
  glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void idle()
{
  srand(time(NULL));
  if(jump != 0)
  {
    g_camera.move_up(fast); m++; jump--;
    usleep(35000);
  }
  if(jump == 0 && jump_end != 0)
  {
    g_camera.move_down(fast); m++; jump_end--;
    usleep(25000);
  }

  rotating += rotate_v;

  curr = std::chrono::system_clock::now();
  std::chrono::duration<float> elaped_seconds = (curr - prev);
  //model_angle += 10 * elaped_seconds.count();
  prev = curr;

  if(j_u != 0 && j_d == 0)
  {
    rabbit.translate += 0.008f*rabbit.front;
    rabbit.translate += 0.008f*rabbit.up;
    rabbit2.translate += 0.007f*rabbit2.front;
    rabbit2.translate += 0.007f*rabbit2.up;
    usleep(30000);
    j_u --;
    if(j_u==0) j_d = 10;
  }
  if(j_d != 0 && j_u == 0)
  {
    rabbit.translate += 0.008f*rabbit.front;
    rabbit.translate -= 0.008f*rabbit.up;
    rabbit2.translate += 0.007f*rabbit2.front;
    rabbit2.translate -= 0.007f*rabbit2.up;
    usleep(30000);
    j_d --;
    if(j_d==0) j_u = 10;
  }
  if((rand()%10)<2 && t_cnt == 10)
  {
    rabbit.angle += 30.0f;
    kmuvcl::math::mat4x4f mat_rotate = kmuvcl::math::rotate(rabbit.angle, 0.0f, 1.0f, 0.0f);
    kmuvcl::math::vec4f r_f(rabbit.front(0), rabbit.front(1), rabbit.front(2), 0);
    r_f = mat_rotate * r_f;
    rabbit.front = kmuvcl::math::vec3f(r_f(0), r_f(1), r_f(2));
  }
  if((rand()%10)<2 && t_cnt == 10)
  {
    rabbit2.angle += 45.0f;
    kmuvcl::math::mat4x4f mat_rotate = kmuvcl::math::rotate(rabbit2.angle, 0.0f, 1.0f, 0.0f);
    kmuvcl::math::vec4f r_f2(rabbit2.front(0), rabbit2.front(1), rabbit2.front(2), 0);
    r_f2 = mat_rotate * r_f2;
    rabbit2.front = kmuvcl::math::vec3f(r_f2(0), r_f2(1), r_f2(2));
  }
  m++;
  if(t_cnt==0) t_cnt = 10;
  else t_cnt --;

  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
  // TODO: properly handle keyboard event
  if(key =='a' || key =='A'){
    g_camera.rotate_left(rotate);
    glutPostRedisplay();
  }
  else if(key =='d' || key =='D'){
    g_camera.rotate_right(rotate);
    glutPostRedisplay();
  }
  else if((key == 'y'|| key == 'Y') && (mode ==1))
  {
      cat.scale -=0.1f;

      glutPostRedisplay();
  }
  else if((key == 'u'|| key == 'U') && (mode ==1))
  {
      cat.scale += 0.1f;

      glutPostRedisplay();
  }
  else if((key == 'o'|| key == 'O') && (mode ==1))
  {
      cat.angle -=10.5f;

      glutPostRedisplay();
  }
  else if((key == 'p'|| key == 'P') && (mode ==1))
  {
      cat.angle += 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'l'|| key == 'L') && (mode ==1))
  {
      cat.translate(0) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'j'|| key == 'J') && (mode ==1))
  {
      cat.translate(0) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'i'|| key == 'I') && (mode ==1))
  {
      cat.translate(2) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'k'|| key == 'K') && (mode ==1))
  {
      cat.translate(2) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'y'|| key == 'Y') && (mode ==2))
  {
      rabbit2.scale -=0.005f;

      glutPostRedisplay();
  }
  else if((key == 'u'|| key == 'U') && (mode ==2))
  {
      rabbit2.scale += 0.005f;

      glutPostRedisplay();
  }
  else if((key == 'o'|| key == 'O') && (mode ==2))
  {
      rabbit2.angle -= 10.5f;
      kmuvcl::math::mat4x4f mat_rotate = kmuvcl::math::rotate(rabbit2.angle, 0.0f, 1.0f, 0.0f);
      kmuvcl::math::vec4f r_f(rabbit2.front(0), rabbit2.front(1), rabbit2.front(2), 0);
      r_f = mat_rotate * r_f;
      glutPostRedisplay();
  }
  else if((key == 'p'|| key == 'P') && (mode ==2))
  {
      rabbit2.angle += 10.5f;
      kmuvcl::math::mat4x4f mat_rotate = kmuvcl::math::rotate(rabbit2.angle, 0.0f, 1.0f, 0.0f);
      kmuvcl::math::vec4f r_f(rabbit2.front(0), rabbit2.front(1), rabbit2.front(2), 0);
      r_f = mat_rotate * r_f;
      glutPostRedisplay();
  }
  else if((key == 'l'|| key == 'L') && (mode ==2))
  {
      rabbit2.translate(0) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'j'|| key == 'J') && (mode ==2))
  {
      rabbit2.translate(0) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'i'|| key == 'I') && (mode ==2))
  {
      rabbit2.translate(2) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'k'|| key == 'K') && (mode ==2))
  {
      rabbit2.translate(2) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'y'|| key == 'Y') && (mode ==3))
  {
      rabbit.scale -=0.1f;

      glutPostRedisplay();
  }
  else if((key == 'u'|| key == 'U') && (mode ==3))
  {
      rabbit.scale += 0.1f;

      glutPostRedisplay();
  }
  else if((key == 'o'|| key == 'O') && (mode ==3))
  {
      rabbit.angle -= 10.5f;
      kmuvcl::math::mat4x4f mat_rotate = kmuvcl::math::rotate(rabbit.angle, 0.0f, 1.0f, 0.0f);
      kmuvcl::math::vec4f r_f(rabbit.front(0), rabbit.front(1), rabbit.front(2), 0);
      r_f = mat_rotate * r_f;

      glutPostRedisplay();
  }
  else if((key == 'p'|| key == 'P') && (mode ==3))
  {
      rabbit.angle += 10.5f;
      kmuvcl::math::mat4x4f mat_rotate = kmuvcl::math::rotate(rabbit.angle, 0.0f, 1.0f, 0.0f);
      kmuvcl::math::vec4f r_f(rabbit.front(0), rabbit.front(1), rabbit.front(2), 0);
      r_f = mat_rotate * r_f;

      glutPostRedisplay();
  }
  else if((key == 'l'|| key == 'L') && (mode ==3))
  {
      rabbit.translate(0) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'j'|| key == 'J') && (mode ==3))
  {
      rabbit.translate(0) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'i'|| key == 'I') && (mode ==3))
  {
      rabbit.translate(2) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'k'|| key == 'K') && (mode ==3))
  {
      rabbit.translate(2) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'y'|| key == 'Y') && (mode ==4))
  {
      giraffe.scale -=0.1f;

      glutPostRedisplay();
  }
  else if((key == 'u'|| key == 'U') && (mode ==4))
  {
      giraffe.scale += 0.1f;

      glutPostRedisplay();
  }
  else if((key == 'o'|| key == 'O') && (mode ==4))
  {
      giraffe.angle -= 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'p'|| key == 'P') && (mode ==4))
  {
      giraffe.angle += 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'l'|| key == 'L') && (mode == 4))
  {
      giraffe.translate(0) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'j'|| key == 'J') && (mode == 4))
  {
      giraffe.translate(0) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'i'|| key == 'I') && (mode == 4))
  {
      giraffe.translate(2) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'k'|| key == 'K') && (mode == 4))
  {
      giraffe.translate(2) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'y'|| key == 'Y') && (mode ==5))
  {
      human.scale -=0.1f;

      glutPostRedisplay();
  }
  else if((key == 'u'|| key == 'U') && (mode ==5))
  {
      human.scale += 0.1f;

      glutPostRedisplay();
  }
  else if((key == 'o'|| key == 'O') && (mode ==5))
  {
      human.angle -= 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'p'|| key == 'P') && (mode ==5))
  {
      human.angle += 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'l'|| key == 'L') && (mode == 5))
  {
      human.translate(0) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'j'|| key == 'J') && (mode == 5))
  {
      human.translate(0) -= 0.5f;

      glutPostRedisplay();

  }
  else if((key == 'i'|| key == 'I') && (mode == 5))
  {
      human.translate(2) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'k'|| key == 'K') && (mode == 5))
  {
      human.translate(2) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'y'|| key == 'Y') && (mode ==6))
  {
      dear.scale -=0.1f;

      glutPostRedisplay();
  }
  else if((key == 'u'|| key == 'U') && (mode ==6))
  {
      dear.scale += 0.1f;

      glutPostRedisplay();
  }
  else if((key == 'o'|| key == 'O') && (mode ==6))
  {
      dear.angle -= 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'p'|| key == 'P') && (mode ==6))
  {
      dear.angle += 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'l'|| key == 'L') && (mode == 6))
  {
      dear.translate(0) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'j'|| key == 'J') && (mode == 6))
  {
      dear.translate(0) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'i'|| key == 'I') && (mode == 6))
  {
      dear.translate(2) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'k'|| key == 'K') && (mode == 6))
  {
      dear.translate(2) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'y'|| key == 'Y') && (mode ==7))
  {
      statue.scale -=0.1f;

      glutPostRedisplay();
  }
  else if((key == 'u'|| key == 'U') && (mode ==7))
  {
      statue.scale += 0.1f;

      glutPostRedisplay();
  }
  else if((key == 'o'|| key == 'O') && (mode ==7))
  {
      statue.angle -= 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'p'|| key == 'P') && (mode ==7))
  {
      statue.angle += 10.5f;

      glutPostRedisplay();
  }
  else if((key == 'l'|| key == 'L') && (mode == 7))
  {
      statue.translate(0) += 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'j'|| key == 'J') && (mode == 7))
  {
      statue.translate(0) -= 0.5f;

      glutPostRedisplay();

  }
  else if((key == 'i'|| key == 'I') && (mode == 7))
  {
      statue.translate(2) -= 0.5f;

      glutPostRedisplay();
  }
  else if((key == 'k'|| key == 'K') && (mode == 7))
  {
      statue.translate(2) += 0.5f;

      glutPostRedisplay();
  }
  else if(key =='w' || key =='W'){
    g_camera.rotate_up(rotate); m++;
    glutPostRedisplay();
  }
  else if(key =='s' || key =='S'){
    g_camera.rotate_down(rotate); m++;
    glutPostRedisplay();
  }
  else if(key == 'z' || key =='Z'){
    jump = 7; jump_end = 7;
    glutPostRedisplay();
  }
  glutPostRedisplay();
}

void special(int key, int x, int y)
{
	// TODO: properly handle special keyboard event
  switch (key){
    case GLUT_KEY_UP:
      g_camera.move_forward(fast); m++;
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      g_camera.move_backward(fast); m++;
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      g_camera.move_left(fast); m++;
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      g_camera.move_right(fast); m++;
      glutPostRedisplay();
      break;
    case GLUT_KEY_F1: //cat
      mode = 1;
      break;
    case GLUT_KEY_F2: //rabbit2
      mode = 2;
      break;
    case GLUT_KEY_F3: //ground2
      mode = 3;
      break;
    case GLUT_KEY_F4: //giraffe
      mode = 4;
      break;
    case GLUT_KEY_F5: //human
      mode = 5;
      break;
    case GLUT_KEY_F6: //dear
      mode = 6;
      break;
    case GLUT_KEY_F7: //statue
      mode = 7;
      break;
    case GLUT_KEY_F8: //rabbit
      mode = 8;
    case GLUT_KEY_F9: //rabbit
      mode = 9;
      break;
  }
  glutPostRedisplay();
}

char temp1[9][30], temp2[9][30];
float x[9], y[9], z[9], angle[9], scale[9];

void Save()
{
  FILE* file;
  file = fopen("Scene.txt", "w");
  fprintf(file, "%s %f %f %f %f %f %s\n", dear.obj_name.c_str(), dear.angle, dear.scale, dear.translate(0), dear.translate(1), dear.translate(2), dear.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", ground.obj_name.c_str(), ground.angle, ground.scale, ground.translate(0), ground.translate(1), ground.translate(2), ground.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", giraffe.obj_name.c_str(), giraffe.angle, dear.scale, giraffe.translate(0), giraffe.translate(1), giraffe.translate(2), giraffe.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", ground2.obj_name.c_str(), dear.angle, ground2.scale, ground2.translate(0), ground2.translate(1), ground2.translate(2), ground2.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", rabbit2.obj_name.c_str(), rabbit2.angle, rabbit2.scale, rabbit2.translate(0), rabbit2.translate(1), rabbit2.translate(2), rabbit2.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", human.obj_name.c_str(), human.angle, human.scale, human.translate(0), human.translate(1), human.translate(2), human.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", cat.obj_name.c_str(), cat.angle, cat.scale, cat.translate(0), cat.translate(1), cat.translate(2), cat.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", statue.obj_name.c_str(), statue.angle, statue.scale, statue.translate(0), statue.translate(1), statue.translate(2), statue.texture_name.c_str());
  fprintf(file, "%s %f %f %f %f %f %s\n", rabbit.obj_name.c_str(), rabbit.angle, cat.scale, rabbit.translate(0), rabbit.translate(1), rabbit.translate(2), rabbit.texture_name.c_str());

  strcpy(temp1[0], dear.obj_name.c_str()); angle[0] = dear.angle; scale[0] = dear.scale; x[0] = dear.translate(0); y[0] = dear.translate(1); z[0] = dear.translate(2); strcpy(temp2[0], dear.texture_name.c_str());
  strcpy(temp1[1], ground.obj_name.c_str());  angle[1] = ground.angle;  scale[1] = ground.scale;  x[1] = ground.translate(0);  y[1] = ground.translate(1);  z[1] = ground.translate(2);  strcpy(temp2[1], ground.texture_name.c_str());
  strcpy(temp1[2], giraffe.obj_name.c_str()); angle[2] = giraffe.angle; scale[2] = giraffe.scale; x[2] = giraffe.translate(0); y[2] = giraffe.translate(1); z[2] = giraffe.translate(2); strcpy(temp2[2], giraffe.texture_name.c_str());
  strcpy(temp1[3], ground2.obj_name.c_str());   angle[3] = ground2.angle;   scale[3] = ground2.scale;   x[3] = ground2.translate(0);   y[3] = ground2.translate(1);   z[3] = ground2.translate(2);   strcpy(temp2[3], ground2.texture_name.c_str());
  strcpy(temp1[4], rabbit2.obj_name.c_str());angle[4] = rabbit2.angle;scale[4] = rabbit2.scale;x[4] = rabbit2.translate(0);y[4] = rabbit2.translate(1);z[4] = rabbit2.translate(2);strcpy(temp2[4], rabbit2.texture_name.c_str());
  strcpy(temp1[5], human.obj_name.c_str());angle[5] = human.angle;scale[5] = human.scale;x[5] = human.translate(0);y[5] = human.translate(1);z[5] = human.translate(2);strcpy(temp2[5], human.texture_name.c_str());
  strcpy(temp1[6], cat.obj_name.c_str());  angle[6] = cat.angle;  scale[6] = cat.scale;  x[6] = cat.translate(0);  y[6] = cat.translate(1);  z[6] = cat.translate(2);  strcpy(temp2[6], cat.texture_name.c_str());
  strcpy(temp1[7], statue.obj_name.c_str());angle[7] = statue.angle;scale[7] = statue.scale;x[7] = statue.translate(0);y[7] = statue.translate(1);z[7] = statue.translate(2);strcpy(temp2[7], statue.texture_name.c_str());
  strcpy(temp1[8], rabbit.obj_name.c_str());  angle[8] = rabbit.angle;  scale[8] = rabbit.scale;  x[8] = rabbit.translate(0);  y[8] = rabbit.translate(1);  z[8] = rabbit.translate(2);  strcpy(temp2[8], rabbit.texture_name.c_str());
}

void Load()
{
  if(cnt==1)
  {
    FILE *file;
    file = fopen("Scene.txt", "r");
    fscanf(file, "%s %f %f %f %f %f %s", temp1[0], &angle[0] ,&scale[0], &x[0], &y[0], &z[0], temp2[0]);
    kmuvcl::math::vec3f d(x[0],y[0],z[0]); dear.translate = d; dear.obj_name = temp1[0]; dear.texture_name = temp2[0]; dear.angle = angle[0]; dear.scale = scale[0];
    fscanf(file, "%s %f %f %f %f %f %s", temp1[1], &angle[1] ,&scale[1], &x[1], &y[1], &z[1], temp2[1]);
    kmuvcl::math::vec3f f(x[1],y[1],z[1]); ground.translate = f; ground.obj_name = temp1[1]; ground.texture_name = temp2[1]; ground.angle = angle[1]; ground.scale = scale[1];
    fscanf(file, "%s %f %f %f %f %f %s", temp1[2], &angle[2] ,&scale[2], &x[2], &y[2], &z[2], temp2[2]);
    kmuvcl::math::vec3f s(x[2],y[2],z[2]); giraffe.translate = s; giraffe.obj_name = temp1[2]; giraffe.texture_name = temp2[2]; giraffe.angle = angle[2]; giraffe.scale = scale[2];
    fscanf(file, "%s %f %f %f %f %f %s", temp1[3], &angle[3] ,&scale[3], &x[3], &y[3], &z[3], temp2[3]);
    kmuvcl::math::vec3f t(x[3],y[3],z[3]); ground2.translate = t; ground2.obj_name = temp1[3]; ground2.texture_name = temp2[3]; ground2.angle = angle[3]; ground2.scale = scale[3];
    fscanf(file, "%s %f %f %f %f %f %s", temp1[4], &angle[4] ,&scale[4], &x[4], &y[4], &z[4], temp2[4]);
    kmuvcl::math::vec3f p(x[4],y[4],z[4]); rabbit2.translate = p; rabbit2.obj_name = temp1[4]; rabbit2.texture_name = temp2[4]; rabbit2.angle = angle[4]; human.scale = scale[4];
    fscanf(file, "%s %f %f %f %f %f %s", temp1[5], &angle[5] ,&scale[5], &x[5], &y[5], &z[5], temp2[5]);
    kmuvcl::math::vec3f h(x[5],y[5],z[5]); human.translate = h; human.obj_name = temp1[5]; human.texture_name = temp2[5]; human.angle = angle[5]; human.scale = scale[5];
    fscanf(file, "%s %f %f %f %f %f %s", temp1[6], &angle[6] ,&scale[6], &x[6], &y[6], &z[6], temp2[6]);
    kmuvcl::math::vec3f c(x[6],y[6],z[6]); cat.translate = c; cat.obj_name = temp1[6]; cat.texture_name = temp2[6]; cat.angle = angle[6]; cat.scale = scale[6];

    fscanf(file, "%s %f %f %f %f %f %s", temp1[7], &angle[7] ,&scale[7], &x[7], &y[7], &z[7], temp2[7]);
    kmuvcl::math::vec3f st(x[7],y[7],z[7]); statue.translate = st; statue.obj_name = temp1[7]; statue.texture_name = temp2[7]; statue.angle = angle[7]; statue.scale = scale[7];
    fscanf(file, "%s %f %f %f %f %f %s", temp1[8], &angle[8] ,&scale[8], &x[6], &y[8], &z[8], temp2[8]);
    kmuvcl::math::vec3f r(x[6],y[6],z[6]); rabbit.translate = r; rabbit.obj_name = temp1[6]; rabbit.texture_name = temp2[6]; rabbit.angle = angle[6]; rabbit.scale = scale[6];
  }
  else
  {
    kmuvcl::math::vec3f d(x[0],y[0],z[0]); dear.translate = d;
    dear.obj_name = temp1[0]; dear.angle = angle[0]; dear.scale = scale[0]; dear.texture_name = temp2[0];
    kmuvcl::math::vec3f f(x[1],y[1],z[1]); ground.translate = f;
    ground.obj_name = temp1[1];  ground.angle = angle[1];  ground.scale = scale[1];  ground.texture_name = temp2[0];
    kmuvcl::math::vec3f s(x[2],y[2],z[2]); giraffe.translate = s;
    giraffe.obj_name = temp1[2]; giraffe.angle = angle[2]; giraffe.scale = scale[2]; giraffe.texture_name = temp2[0];
    kmuvcl::math::vec3f t(x[3],y[3],z[3]); ground2.translate = t;
    ground2.obj_name = temp1[3];    ground2.angle = angle[3];   ground2.scale = scale[3];   ground2.texture_name = temp2[0];
    kmuvcl::math::vec3f p(x[4],y[4],z[4]); rabbit2.translate = p;
    rabbit2.obj_name = temp1[4]; rabbit2.angle = angle[4];rabbit2.scale = scale[4];rabbit2.texture_name = temp2[0];
    kmuvcl::math::vec3f h(x[5],y[5],z[5]); human.translate = h;
    human.obj_name = temp1[5]; human.angle = angle[5];human.scale = scale[5];human.texture_name = temp2[0];
    kmuvcl::math::vec3f c(x[6],y[6],z[6]); cat.translate = c;
    cat.obj_name = temp1[6];   cat.angle = angle[6];  cat.scale = scale[6];  cat.texture_name = temp2[0];

    kmuvcl::math::vec3f st(x[7],y[7],z[7]); statue.translate = st;
    statue.obj_name = temp1[7]; statue.angle = angle[7];statue.scale = scale[7];statue.texture_name = temp2[0];
    kmuvcl::math::vec3f r(x[8],y[8],z[8]); rabbit.translate = r;
    rabbit.obj_name = temp1[8];   rabbit.angle = angle[8];  rabbit.scale = scale[8];  rabbit.texture_name = temp2[0];
  }
}
void main_menus(int values)
{
   switch (values)
   {
     case 1: Load(); m++; break;
     case 2: Save(); m++; break;
   }
   glutPostRedisplay();
}

void create_GLU_menus()
{
  glutCreateMenu(main_menus);
  glutAddMenuEntry("Load",1);
  glutAddMenuEntry("Save",2);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
