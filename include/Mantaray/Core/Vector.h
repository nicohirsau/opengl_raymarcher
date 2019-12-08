#pragma once

namespace MR {
class Vector2f {
    public:
        Vector2f() {}
        Vector2f(float x, float y) {
            this->x = x;
            this->y = y;
        }

        bool operator==(Vector2f b) {
            return (this->x == b.x && this->y == b.y);
        }
        Vector2f operator+(Vector2f b) {
            return Vector2f(this->x + b.x, this->y + b.y);
        }
        Vector2f operator-(Vector2f b) {
            return Vector2f(this->x - b.x, this->y - b.y);
        }
        Vector2f operator*(float b) {
            return Vector2f(this->x * b, this->y * b);
        }

    public:
        float x, y = 0.0f;
};

class Vector2d {
    public:
        Vector2d() {}
        Vector2d(double x, double y) {
            this->x = x;
            this->y = y;
        }

        bool operator==(Vector2d b) {
            return (this->x == b.x && this->y == b.y);
        }
        Vector2d operator+(Vector2d b) {
            return Vector2d(this->x + b.x, this->y + b.y);
        }
        Vector2d operator-(Vector2d b) {
            return Vector2d(this->x - b.x, this->y - b.y);
        }

    public:
        double x, y = 0.0f;
};

class Vector2i {
    public:
        Vector2i() {}
        Vector2i(int x, int y) {
            this->x = x;
            this->y = y;
        }

    public:
        int x, y = 0;
};

class Vector3f {
    public:
        Vector3f() {}
        Vector3f(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        bool operator==(Vector3f b) {
            return (this->x == b.x && this->y == b.y && this->z == b.z);
        }
        Vector3f operator+(Vector3f b) {
            return Vector3f(this->x + b.x, this->y + b.y, this->z + b.z);
        }
        Vector3f operator-(Vector3f b) {
            return Vector3f(this->x - b.x, this->y - b.y, this->z - b.z);
        }
        Vector3f operator*(float b) {
            return Vector3f(this->x * b, this->y * b, this->y * b);
        }
        //void rotateAroundX(float angle) {
        //    this->x = this->x;
        //    this->y = this->y * cos(angle) - this->z * sin(angle);
        //    this->z = this->y * sin(angle) + this->z * cos(angle);
        //}
//
        //void rotateAroundY(float angle) {
        //    this->x = this->x * cos(angle) + this->z * sin(angle);
        //    this->y = this->y;
        //    this->z = -this->x * sin(angle) + this->z * cos(angle);
        //}
    
    public:
        float x, y, z = 0.0f;
};
}