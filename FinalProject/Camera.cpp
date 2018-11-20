#include "Camera.h"


const kmuvcl::math::vec3f Camera::center_position() const
{
  kmuvcl::math::vec3f center_position(position_(0) + front_dir_(0),
                 position_(1) + front_dir_(1),
                 position_(2) + front_dir_(2));
  return center_position;
}

// TODO: fill up the following functions properly
void Camera::move_up(float delta)
{
  position_ += delta * up_dir_;
}
void Camera::move_down(float delta)
{
  position_ -= delta * up_dir_;
}
void Camera::move_forward(float delta)
{
  //kmuvcl::math::vec3f temp(0.0f, 0.0f, delta);
  position_ += delta * front_dir_;
}

void Camera::move_backward(float delta)
{
  //kmuvcl::math::vec3f temp(0.0f, 0.0f, delta);
  position_ -= delta * front_dir_;
}

void Camera::move_left(float delta)
{
  //kmuvcl::math::vec3f temp(delta, 0.0f, 0.0f);
  position_ -= delta * right_dir_;
}

void Camera::move_right(float delta)
{
  //kmuvcl::math::vec3f temp(delta, 0.0f, 0.0f);
  position_ += delta * right_dir_;
}

void Camera::rotate_left(float delta)
{
  kmuvcl::math::vec4f temp1(front_dir_(0), front_dir_(1), front_dir_(2), 0.0f);
  kmuvcl::math::vec4f temp2(right_dir_(0), right_dir_(1), right_dir_(2), 0.0f);
  kmuvcl::math::mat4x4f rotateMat, modelMat;

  rotateMat = kmuvcl::math::rotate(delta, 0.0f, 1.0f, 0.0f);
  temp1 = rotateMat * temp1;
  temp2 = rotateMat * temp2;

  for(int i=0; i<3; i++)
  {
    front_dir_(i) = temp1(i);
    right_dir_(i) = temp2(i);
  }
}

void Camera::rotate_right(float delta)
{
  kmuvcl::math::vec4f temp1(front_dir_(0), front_dir_(1), front_dir_(2), 0.0f);
  kmuvcl::math::vec4f temp2(right_dir_(0), right_dir_(1), right_dir_(2), 0.0f);
  kmuvcl::math::mat4x4f rotateMat, modelMat;

  rotateMat = kmuvcl::math::rotate(-delta, 0.0f, 1.0f, 0.0f);
  temp1 = rotateMat * temp1;
  temp2 = rotateMat * temp2;

  for(int i=0; i<3; i++)
  {
    front_dir_(i) = temp1(i);
    right_dir_(i) = temp2(i);
  }
}

void Camera::rotate_up(float delta)
{
  kmuvcl::math::vec4f temp1(front_dir_(0), front_dir_(1), front_dir_(2), 0.0f);
  kmuvcl::math::vec4f temp2(right_dir_(0), right_dir_(1), right_dir_(2), 0.0f);
  kmuvcl::math::mat4x4f rotateMat, modelMat;

  rotateMat = kmuvcl::math::rotate(delta, right_dir_(0), right_dir_(1), right_dir_(2));
  temp1 = rotateMat * temp1;
  temp2 = rotateMat * temp2;

  for(int i=0; i<3; i++)
  {
    front_dir_(i) = temp1(i);
    right_dir_(i) = temp2(i);
  }
}

void Camera::rotate_down(float delta)
{
  kmuvcl::math::vec4f temp1(front_dir_(0), front_dir_(1), front_dir_(2), 0.0f);
  kmuvcl::math::vec4f temp2(right_dir_(0), right_dir_(1), right_dir_(2), 0.0f);
  kmuvcl::math::mat4x4f rotateMat, modelMat;

  rotateMat = kmuvcl::math::rotate(-delta, right_dir_(0), right_dir_(1), right_dir_(2));
  temp1 = rotateMat * temp1;
  temp2 = rotateMat * temp2;

  for(int i=0; i<3; i++)
  {
    front_dir_(i) = temp1(i);
    right_dir_(i) = temp2(i);
  }
}
