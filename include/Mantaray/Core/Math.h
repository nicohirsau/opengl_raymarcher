namespace MR {
class Vector2f {
    public:
        Vector2f() {}
        Vector2f(float x, float y) {
            this->x = x;
            this->y = y;
        }
        float x, y = 0.0f;
};

class Vector2d {
    public:
        Vector2d() {}
        Vector2d(double x, double y) {
            this->x = x;
            this->y = y;
        }
        double x, y = 0.0f;
};
}