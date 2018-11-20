#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include "SOIL.h"

#include "Object.h"

Group::Group(const std::string& name)
{
  m_name = name;

  m_vertices.clear();
  m_normals.clear();
  m_texcoords.clear();

  m_mtl_name = "Default";
}

Material::Material()
{
  m_name = "Default";

  m_ambient   = kmuvcl::math::vec4f(0.5f, 0.5f, 0.5f, 1.0f);
  m_diffuse   = kmuvcl::math::vec4f(0.5f, 0.5f, 0.5f, 1.0f);
  m_specular  = kmuvcl::math::vec4f(0.5f, 0.5f, 0.5f, 1.0f);
  m_shininess = 1.0f;
}

Material::Material(const std::string& name,
  kmuvcl::math::vec4f& ambient,
  kmuvcl::math::vec4f& diffuse,
  kmuvcl::math::vec4f& specular,
  float& shininess)
{
  m_name = name;

  m_ambient   = ambient;
  m_diffuse   = diffuse;
  m_specular  = specular;
  m_shininess = shininess;
}

Object::Object(){
  minX=minY=minZ = 100.0f;
  maxX=maxY=maxZ = -100.0f;
}

void Object::draw(int loc_a_vertex, int loc_a_normal,
  int loc_u_material_ambient, int loc_u_material_diffuse,
  int loc_u_material_specular, int loc_u_material_shininess)
{
  for (size_t i=0; i<m_groups.size(); ++i)
  {
    Group& group = m_groups[i];

    Material& mtl = m_materials[group.m_mtl_name];
    kmuvcl::math::vec4f& ambient  = mtl.m_ambient;
    kmuvcl::math::vec4f& diffuse  = mtl.m_diffuse;
    kmuvcl::math::vec4f& specular = mtl.m_specular;
    float& shininess              = mtl.m_shininess;

    glUniform4fv(loc_u_material_ambient, 1, ambient);
    glUniform4fv(loc_u_material_diffuse, 1, diffuse);
    glUniform4fv(loc_u_material_specular, 1, specular);
    glUniform1f(loc_u_material_shininess, shininess);

    glVertexAttribPointer(loc_a_vertex, 3, GL_FLOAT, false, 0, group.m_vertices.data());
    glVertexAttribPointer(loc_a_normal, 3, GL_FLOAT, false, 0, group.m_normals.data());

    glEnableVertexAttribArray(loc_a_vertex);
    glEnableVertexAttribArray(loc_a_normal);

    glDrawArrays(GL_TRIANGLES, 0, group.m_vertices.size());

    glDisableVertexAttribArray(loc_a_vertex);
    glDisableVertexAttribArray(loc_a_normal);
  }
}

void Object::draw(int loc_a_vertex, int loc_a_normal,  int loc_a_texcoord,
    int loc_u_material_ambient, int loc_u_material_diffuse,
    int loc_u_material_specular, int loc_u_material_shininess)
{
  for (size_t i=0; i<m_groups.size(); ++i)
  {
    Group& group = m_groups[i];

    Material& mtl = m_materials[group.m_mtl_name];
    kmuvcl::math::vec4f& ambient  = mtl.m_ambient;
    kmuvcl::math::vec4f& diffuse  = mtl.m_diffuse;
    kmuvcl::math::vec4f& specular = mtl.m_specular;
    float& shininess              = mtl.m_shininess;

    glUniform4fv(loc_u_material_ambient, 1, ambient);
    glUniform4fv(loc_u_material_diffuse, 1, diffuse);
    glUniform4fv(loc_u_material_specular, 1, specular);
    glUniform1f(loc_u_material_shininess, shininess);

    glVertexAttribPointer(loc_a_vertex, 3, GL_FLOAT, false, 0, group.m_vertices.data());
    glVertexAttribPointer(loc_a_normal, 3, GL_FLOAT, false, 0, group.m_normals.data());
    glVertexAttribPointer(loc_a_texcoord, 2, GL_FLOAT, false, 0, group.m_texcoords.data());

    glEnableVertexAttribArray(loc_a_vertex);
    glEnableVertexAttribArray(loc_a_normal);
    glEnableVertexAttribArray(loc_a_texcoord);

    glDrawArrays(GL_TRIANGLES, 0, group.m_vertices.size());

    glDisableVertexAttribArray(loc_a_vertex);
    glDisableVertexAttribArray(loc_a_normal);
    glDisableVertexAttribArray(loc_a_texcoord);
  }
}

void Object::print()
{
  for (size_t i = 0; i < m_groups.size(); ++i)
  {
    Group& group = m_groups[i];

    Material& mtl = m_materials[group.m_mtl_name];
    kmuvcl::math::vec4f& ambient = mtl.m_ambient;
    kmuvcl::math::vec4f& diffuse = mtl.m_diffuse;
    kmuvcl::math::vec4f& specular = mtl.m_specular;
    float& shininess = mtl.m_shininess;

    std::cout << "Group: " << group.m_name << std::endl;

    std::cout << "\tUsemtl: " << group.m_mtl_name << std::endl;
    std::cout << "\t\tAmbient: " << ambient[0] << " " << ambient[1] << " " << ambient[2] << std::endl;
    std::cout << "\t\tDiffuse: " << diffuse[0] << " " << diffuse[1] << " " << diffuse[2] << std::endl;
    std::cout << "\t\tSpecular: " << specular[0] << " " << specular[1] << " " << specular[2] << std::endl;
    std::cout << "\t\tShininess: " << shininess << std::endl;

    std::cout << "\tVertices: " << group.m_vertices.size() << std::endl;
    for (size_t i = 0; i < group.m_vertices.size(); ++i)
    {
      std::cout << "\t\t" << group.m_vertices[i][0] << " " << group.m_vertices[i][1] << " " << group.m_vertices[i][2] << std::endl;

    }
    std::cout << "\tTexcoord: " << group.m_texcoords.size() << std::endl;
    for (size_t i = 0; i < group.m_texcoords.size(); ++i)
    {
      std::cout << "\t\t" << group.m_texcoords[i][0] << " " << group.m_texcoords[i][1] << std::endl;
    }

    std::cout << "\tNormal: " << group.m_normals.size() << std::endl;
    for (size_t i = 0; i < group.m_normals.size(); ++i)
    {
      std::cout << "\t\t" << group.m_normals[i][0] << " " << group.m_normals[i][1] << " " << group.m_normals[i][2] << std::endl;
    }
  }
}

bool Object::load_simple_obj(const std::string& filename)
{
	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "failed to open file: " << filename << std::endl;
		return false;
	}

  std::string type_str;
  char slash;				// get only on character '\'

  std::string line;
  std::locale loc;

  bool has_texcoord = false;
  bool has_normal   = false;;

	std::vector<kmuvcl::math::vec3f> vertices;
	std::vector<kmuvcl::math::vec2f> texcoords;
	std::vector<kmuvcl::math::vec3f> normals;

  m_groups.push_back(Group("Default"));
  m_materials["Default"] = Material();

  PATH = filename.substr(0, filename.find_last_of('/'));

  std::stringstream ss;

	while (!file.eof())
	{
		std::getline(file, line);

		ss.clear();
		ss.str(line);

		// comment or space
		if (line[0] == '#' || std::isspace(line[0], loc))
		{
			continue; // skip
		}
    else if (line.substr(0, 7) == "mtllib ") // open material file
    {
      std::string mtl_filename;
      ss >> type_str >> mtl_filename;

      std::string fullpath = PATH + '/' + mtl_filename;
      load_simple_mtl(fullpath);
    }
		else if (line.substr(0, 2) == "v ")   // vertex
		{
			kmuvcl::math::vec3f vertex;
			ss >> type_str >> vertex[0] >> vertex[1] >> vertex[2];

      //get boundary
      if(minX>vertex[0]) minX = vertex[0];
      if(minY>vertex[1]) minY = vertex[1];
      if(minZ>vertex[2]) minZ = vertex[2];

      if(maxX<vertex[0]) maxX = vertex[0];
      if(maxY<vertex[1]) maxY = vertex[1];
      if(maxZ<vertex[2]) maxZ = vertex[2];

			vertices.push_back(vertex);
		}
		else if (line.substr(0, 3) == "vt ")  // texture coordinate
		{
			kmuvcl::math::vec2f texcoord;
			ss >> type_str >> texcoord[0] >> texcoord[1];
			texcoords.push_back(texcoord);

      has_texcoord = true;
		}
		else if (line.substr(0, 3) == "vn ")  // vertex normal
		{
			kmuvcl::math::vec3f norm;
			ss >> type_str >> norm[0] >> norm[1] >> norm[2];
			normals.push_back(norm);

      has_normal = true;
		}
    else if (line.substr(0, 2) == "g ")   // group
    {
      std::string group_name;
      ss >> type_str >> group_name;

      m_groups.push_back(Group(group_name));
    }
    else if (line.substr(0, 7) == "usemtl ") // materail
    {
      std::string mtl_name;
      ss >> type_str >> mtl_name;

      m_groups.back().m_mtl_name = mtl_name;
    }
		// faces
		else if (line.substr(0, 2) == "f ")
		{
      Group& group = m_groups.back();

      std::vector<std::string> lines;

      kmuvcl::math::vec3i vertex_idx;
      kmuvcl::math::vec3i texcoord_idx;
      kmuvcl::math::vec3i normal_idx;

      if (!has_texcoord && !has_normal) // f v
      {
        ss >> type_str >> vertex_idx[0] >> vertex_idx[1] >> vertex_idx[2];

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);
      }
      else if (has_texcoord && !has_normal) // f v/vt
      {
        ss >> type_str >>
          vertex_idx[0] >> slash >> texcoord_idx[0] >>
          vertex_idx[1] >> slash >> texcoord_idx[1] >>
          vertex_idx[2] >> slash >> texcoord_idx[2];

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);

        group.m_texcoords.push_back(texcoords[texcoord_idx[0] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[1] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[2] - 1]);
      }
      else if (!has_texcoord && has_normal) // f v//vn
      {
        ss >> type_str >>
          vertex_idx[0] >> slash >> slash >> normal_idx[0] >>
          vertex_idx[1] >> slash >> slash >> normal_idx[1] >>
          vertex_idx[2] >> slash >> slash >> normal_idx[2];

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);

        group.m_normals.push_back(normals[normal_idx[0] - 1]);
        group.m_normals.push_back(normals[normal_idx[1] - 1]);
        group.m_normals.push_back(normals[normal_idx[2] - 1]);
      }
      else if (has_texcoord && has_normal) // f v/vt/vn
      {
        ss >> type_str >>
          vertex_idx[0] >> slash >> texcoord_idx[0] >> slash >> normal_idx[0] >>
          vertex_idx[1] >> slash >> texcoord_idx[1] >> slash >> normal_idx[1] >>
          vertex_idx[2] >> slash >> texcoord_idx[2] >> slash >> normal_idx[2];

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);

        group.m_texcoords.push_back(texcoords[texcoord_idx[0] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[1] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[2] - 1]);

        group.m_normals.push_back(normals[normal_idx[0] - 1]);
        group.m_normals.push_back(normals[normal_idx[1] - 1]);
        group.m_normals.push_back(normals[normal_idx[2] - 1]);
      }
    }
	}

	std::cout << "finished to read: " << filename << std::endl;
	return true;
}

bool Object::load_simple_mtl(const std::string& filename)
{
  std::ifstream file(filename.c_str());

  if (!file.is_open())
  {
    std::cerr << "failed to open file: " << filename << std::endl;
    return false;
  }

  float r, g, b;
  std::string type_str;

  std::string line;
  std::locale loc;
  std::string KDpath;

  std::string           name;
  kmuvcl::math::vec4f   ambient;
  kmuvcl::math::vec4f   diffuse;
  kmuvcl::math::vec4f   specular;
  float                 shininess;

  bool is_name = false;
  bool is_ka   = false;
  bool is_kd   = false;
  bool is_ks   = false;
  bool is_ns   = false;

  std::stringstream ss;

  while (!file.eof())
  {
    std::getline(file, line);

    ss.clear();
    ss.str(line);

    // comment or space
    if (line[0] == '#' || std::isspace(line[0], loc))
    {
      continue; // skip
    }
    else if (line.substr(0, 7) == "newmtl ")
    {
        ss >> type_str >> name;
        is_name = true;
    }
    else if (line.substr(0, 3) == "Ka ")
    {
      ss >> type_str >> r >> g >> b;
      ambient = kmuvcl::math::vec4f(r, g, b, 1.0f);
      is_ka = true;
    }
    else if (line.substr(0, 3) == "Kd ")
    {
      ss >> type_str >> r >> g >> b;
      diffuse = kmuvcl::math::vec4f(r, g, b, 1.0f);
      is_kd = true;
    }
    else if (line.substr(0, 3) == "Ks ")
    {
      ss >> type_str >> r >> g >> b;
      specular = kmuvcl::math::vec4f(r, g, b, 1.0f);
      is_ks = true;
    }
    else if (line.substr(0, 3) == "Ns ")
    {
      ss >> type_str >> shininess;
      is_ns = true;
    }
    else if (line.substr(0, 7) == "map_Kd ")
    {
      ss >> type_str >> KDpath;
      std::string fullpath = PATH + '/' + KDpath;
      load_texture(fullpath);
    }

    if (is_name == true && is_ka == true && is_kd == true && is_ks == true && is_ns == true)
    {
      m_materials[name] = Material(name, ambient, diffuse, specular, shininess);

      is_name = false;
      is_ka = false;
      is_kd = false;
      is_ks = false;
      is_ns = false;
    }
  }
}

void Object::load_texture(const std::string& filename)
{
  std::ifstream file(filename.c_str());

  textureid = SOIL_load_OGL_texture
  (
    filename.c_str(),
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );

  if (textureid == 0)
  {
    std::cerr << "Fail to load an image file with SOIL_load_OGL_texture() function." << std::endl;
    return;
  }

  glBindTexture(GL_TEXTURE_2D, textureid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Object::make_boundary()
{
  kmuvcl::math::vec4f temp0(minX, minY, minZ); V[0] = temp0;
  kmuvcl::math::vec4f temp1(minX, minY, maxZ); V[1] = temp1;
  kmuvcl::math::vec4f temp2(maxX, minY, maxZ); V[2] = temp2;
  kmuvcl::math::vec4f temp3(maxX, minY, minZ); V[3] = temp3;
  kmuvcl::math::vec4f temp4(minX, maxY, minZ); V[4] = temp4;
  kmuvcl::math::vec4f temp5(minX, maxY, maxZ); V[5] = temp5;
  kmuvcl::math::vec4f temp6(maxX, maxY, maxZ); V[6] = temp6;
  kmuvcl::math::vec4f temp7(maxX, maxY, minZ); V[7] = temp7;
}
