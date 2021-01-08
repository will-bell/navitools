struct Vector2 {
    Vector2(double x, double y): x(x), y(y) {}
    double x;
    double y;
};

struct Vector3 {
    Vector3(double x, double y, double z): x(x), y(y), z(z) {}
    double x;
    double y;
    double z;
};

double magnitude(const Vector2& u);
Vector2 scale(const Vector2& u, double alpha);
Vector2 normalize(const Vector2& u);
Vector2 add(const Vector2& u, const Vector2& v);
Vector2 subtract(const Vector2& u, const Vector2& v);

double magnitude(const Vector3& u);
Vector3 scale(const Vector3& u, double alpha);
Vector3 normalize(const Vector3& u);
Vector3 add(const Vector3& u, const Vector3& v);
Vector3 subtract(const Vector3& u, const Vector3& v);
