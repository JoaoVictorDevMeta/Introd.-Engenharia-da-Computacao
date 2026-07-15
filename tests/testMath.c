#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "vector3d.h"
#include "quaterniom.h"
#include "matriz3x3.h"

// Suite de testes com assert.h para vec3, quat e mat3.
// Cobre principalmente os casos mais sensíveis a erro numérico:
// quat_normalize, mat3_inverse e mat3_from_quat.
// Serve de exemplo para utilização nos códigos.

#define EPS 1e-9
#define PI 3.14159265358979323846

static int approx_equal(double a, double b, double eps) {
    return fabs(a - b) < eps;
}

static int vec3_equal(Vec3 a, Vec3 b, double eps) {
    return approx_equal(a.x, b.x, eps) &&
           approx_equal(a.y, b.y, eps) &&
           approx_equal(a.z, b.z, eps);
}

static int quat_equal(Quat a, Quat b, double eps) {
    return approx_equal(a.w, b.w, eps) &&
           approx_equal(a.x, b.x, eps) &&
           approx_equal(a.y, b.y, eps) &&
           approx_equal(a.z, b.z, eps);
}

static int mat3_equal(Mat3 a, Mat3 b, double eps) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!approx_equal(a.m[i][j], b.m[i][j], eps)) return 0;
        }
    }
    return 1;
}

// ---------------------------------------------------------------------
// vec3
// ---------------------------------------------------------------------

static void test_vec3_add_sub_scale(void) {
    Vec3 a = {1.0, 2.0, 3.0};
    Vec3 b = {4.0, 5.0, 6.0};

    assert(vec3_equal(vec3_add(a, b), (Vec3){5.0, 7.0, 9.0}, EPS));
    assert(vec3_equal(vec3_sub(a, b), (Vec3){-3.0, -3.0, -3.0}, EPS));
    assert(vec3_equal(vec3_scale(a, 2.0), (Vec3){2.0, 4.0, 6.0}, EPS));
    assert(vec3_equal(vec3_scale(a, 0.0), (Vec3){0.0, 0.0, 0.0}, EPS));
}

static void test_vec3_dot(void) {
    Vec3 a = {1.0, 2.0, 3.0};
    Vec3 b = {4.0, -5.0, 6.0};

    assert(approx_equal(vec3_dot(a, b), 1*4 + 2*(-5) + 3*6, EPS));

    Vec3 x = {1.0, 0.0, 0.0};
    Vec3 y = {0.0, 1.0, 0.0};
    assert(approx_equal(vec3_dot(x, y), 0.0, EPS)); // vetores ortogonais
}

static void test_vec3_product(void) {
    Vec3 x = {1.0, 0.0, 0.0};
    Vec3 y = {0.0, 1.0, 0.0};
    Vec3 z = {0.0, 0.0, 1.0};

    // x cross y = z (regra da mão direita)
    assert(vec3_equal(vec3_product(x, y), z, EPS));
    assert(vec3_equal(vec3_product(y, z), x, EPS));
    assert(vec3_equal(vec3_product(z, x), y, EPS));

    // produto vetorial de um vetor com ele mesmo é nulo
    Vec3 v = {3.0, -2.0, 5.0};
    assert(vec3_equal(vec3_product(v, v), (Vec3){0.0, 0.0, 0.0}, EPS));

    // resultado deve ser ortogonal aos dois vetores originais
    Vec3 a = {1.0, 2.0, 3.0};
    Vec3 b = {4.0, 5.0, 6.0};
    Vec3 c = vec3_product(a, b);
    assert(approx_equal(vec3_dot(c, a), 0.0, EPS));
    assert(approx_equal(vec3_dot(c, b), 0.0, EPS));
}

static void test_vec3_length_and_normalize(void) {
    Vec3 v = {3.0, 4.0, 0.0};
    assert(approx_equal(vec3_length(v), 5.0, EPS));

    Vec3 n = vec3_normalize(v);
    assert(approx_equal(vec3_length(n), 1.0, EPS));
    assert(vec3_equal(n, (Vec3){0.6, 0.8, 0.0}, EPS));

    // vetor nulo deve normalizar para (0,0,0), sem divisão por zero
    Vec3 zero = {0.0, 0.0, 0.0};
    assert(vec3_equal(vec3_normalize(zero), (Vec3){0.0, 0.0, 0.0}, EPS));
}

static void test_vec3_distance(void) {
    Vec3 a = {1.0, 2.0, 3.0};
    Vec3 b = {1.0, 2.0, 3.0};
    assert(approx_equal(vec3_distance(a, b), 0.0, EPS));

    Vec3 c = {0.0, 0.0, 0.0};
    Vec3 d = {3.0, 4.0, 0.0};
    assert(approx_equal(vec3_distance(c, d), 5.0, EPS));
}

static void test_vec3_torque(void) {
    Vec3 r = {1.0, 0.0, 0.0};
    Vec3 F = {0.0, 1.0, 0.0};
    // torque = r x F, deve coincidir com o produto vetorial
    assert(vec3_equal(vec3_torque(r, F), vec3_product(r, F), EPS));
}

static void test_vec3(void) {
    test_vec3_add_sub_scale();
    test_vec3_dot();
    test_vec3_product();
    test_vec3_length_and_normalize();
    test_vec3_distance();
    test_vec3_torque();
    printf("[OK] vec3\n");
}

// ---------------------------------------------------------------------
// quat
// ---------------------------------------------------------------------

static void test_quat_add_scale(void) {
    Quat a = {1.0, 2.0, 3.0, 4.0};
    Quat b = {4.0, 3.0, 2.0, 1.0};

    assert(quat_equal(quat_add(a, b), (Quat){5.0, 5.0, 5.0, 5.0}, EPS));
    assert(quat_equal(quat_scale(a, 2.0), (Quat){2.0, 4.0, 6.0, 8.0}, EPS));
    assert(quat_equal(quat_scale(a, 0.0), (Quat){0.0, 0.0, 0.0, 0.0}, EPS));
}

static void test_quat_mul(void) {
    Quat identity = {1.0, 0.0, 0.0, 0.0};
    Quat q = {0.5, 0.5, 0.5, 0.5};

    // multiplicação pela identidade não altera o quatérnio
    assert(quat_equal(quat_mul(identity, q), q, EPS));
    assert(quat_equal(quat_mul(q, identity), q, EPS));

    // duas rotações de 90 graus em torno de Z combinadas = rotação de 180 graus em Z
    Quat rot90z = quat_from_axis_angle((Vec3){0.0, 0.0, 1.0}, PI / 2.0);
    Quat rot180z = quat_from_axis_angle((Vec3){0.0, 0.0, 1.0}, PI);
    Quat combined = quat_mul(rot90z, rot90z);
    assert(quat_equal(combined, rot180z, 1e-9) ||
           quat_equal(quat_scale(combined, -1.0), rot180z, 1e-9)); // +/- q representam a mesma rotação
}

static void test_quat_conjugate(void) {
    Quat q = {1.0, 2.0, 3.0, 4.0};
    Quat c = quat_conjugate(q);
    assert(quat_equal(c, (Quat){1.0, -2.0, -3.0, -4.0}, EPS));

    // q * conjugado(q) deve resultar num quatérnio puramente escalar = |q|^2
    Quat prod = quat_mul(q, c);
    double norm_sq = q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z;
    assert(approx_equal(prod.w, norm_sq, 1e-9));
    assert(approx_equal(prod.x, 0.0, 1e-9));
    assert(approx_equal(prod.y, 0.0, 1e-9));
    assert(approx_equal(prod.z, 0.0, 1e-9));
}

static void test_quat_normalize(void) {
    Quat q = {2.0, 0.0, 0.0, 0.0};
    Quat n = quat_normalize(q);
    assert(quat_equal(n, (Quat){1.0, 0.0, 0.0, 0.0}, EPS));

    Quat q2 = {1.0, 2.0, 3.0, 4.0};
    Quat n2 = quat_normalize(q2);
    double norm = sqrt(n2.w*n2.w + n2.x*n2.x + n2.y*n2.y + n2.z*n2.z);
    assert(approx_equal(norm, 1.0, 1e-9));

    // quatérnio nulo deve normalizar para a rotação identidade, sem divisão por zero
    Quat zero = {0.0, 0.0, 0.0, 0.0};
    Quat n3 = quat_normalize(zero);
    assert(quat_equal(n3, (Quat){1.0, 0.0, 0.0, 0.0}, EPS));

    // quatérnio já unitário deve permanecer (praticamente) inalterado
    Quat unit = quat_from_axis_angle((Vec3){0.0, 1.0, 0.0}, 1.234);
    Quat n4 = quat_normalize(unit);
    assert(quat_equal(unit, n4, 1e-9));
}

static void test_quat_from_omega(void) {
    Vec3 omega = {1.0, 2.0, 3.0};
    Quat q = quat_from_omega(omega);
    assert(approx_equal(q.w, 0.0, EPS));
    assert(vec3_equal((Vec3){q.x, q.y, q.z}, omega, EPS));
}

static void test_quat_from_axis_angle(void) {
    // ângulo zero deve gerar a rotação identidade
    Quat q0 = quat_from_axis_angle((Vec3){0.0, 0.0, 1.0}, 0.0);
    assert(quat_equal(q0, (Quat){1.0, 0.0, 0.0, 0.0}, 1e-9));

    // rotação de 90 graus em torno de Z: w = cos(45deg), z = sin(45deg)
    Quat q90 = quat_from_axis_angle((Vec3){0.0, 0.0, 1.0}, PI / 2.0);
    double c = cos(PI / 4.0);
    double s = sin(PI / 4.0);
    assert(quat_equal(q90, (Quat){c, 0.0, 0.0, s}, 1e-9));

    // todo quatérnio de eixo/ângulo com eixo unitário já deve ter norma 1
    double norm = sqrt(q90.w*q90.w + q90.x*q90.x + q90.y*q90.y + q90.z*q90.z);
    assert(approx_equal(norm, 1.0, 1e-9));
}

static void test_quat(void) {
    test_quat_add_scale();
    test_quat_mul();
    test_quat_conjugate();
    test_quat_normalize();
    test_quat_from_omega();
    test_quat_from_axis_angle();
    printf("[OK] quat\n");
}

// ---------------------------------------------------------------------
// mat3
// ---------------------------------------------------------------------

static void test_mat3_identity(void) {
    Mat3 id = mat3_identity();
    Vec3 v = {3.0, -1.0, 7.0};
    assert(vec3_equal(mat3_mul_vec3(id, v), v, EPS));
    assert(mat3_equal(mat3_mul(id, id), id, EPS));
}

static void test_mat3_rotations(void) {
    // rotação de 90 graus em X leva Y -> Z
    Mat3 rx = mat3_rotation_x(PI / 2.0);
    Vec3 y = {0.0, 1.0, 0.0};
    assert(vec3_equal(mat3_mul_vec3(rx, y), (Vec3){0.0, 0.0, 1.0}, 1e-9));

    // rotação de 90 graus em Y leva Z -> X
    Mat3 ry = mat3_rotation_y(PI / 2.0);
    Vec3 z = {0.0, 0.0, 1.0};
    assert(vec3_equal(mat3_mul_vec3(ry, z), (Vec3){1.0, 0.0, 0.0}, 1e-9));

    // rotação de 90 graus em Z leva X -> Y
    Mat3 rz = mat3_rotation_z(PI / 2.0);
    Vec3 x = {1.0, 0.0, 0.0};
    assert(vec3_equal(mat3_mul_vec3(rz, x), (Vec3){0.0, 1.0, 0.0}, 1e-9));
}

static void test_mat3_mul_and_mul_vec3(void) {
    Mat3 rx = mat3_rotation_x(PI / 2.0);
    Mat3 id = mat3_identity();

    assert(mat3_equal(mat3_mul(rx, id), rx, 1e-9));
    assert(mat3_equal(mat3_mul(id, rx), rx, 1e-9));

    // aplicar rx duas vezes via mat3_mul deve equivaler a uma rotação de 180 graus em X
    Mat3 rx180 = mat3_rotation_x(PI);
    assert(mat3_equal(mat3_mul(rx, rx), rx180, 1e-9));
}

static void test_mat3_transpose(void) {
    Mat3 rz = mat3_rotation_z(0.7);
    Mat3 t = mat3_transpose(rz);

    // para matrizes de rotação, a transposta é igual à inversa: R * R^T = I
    Mat3 result = mat3_mul(rz, t);
    assert(mat3_equal(result, mat3_identity(), 1e-9));
}

static void test_mat3_inverse(void) {
    // inversa da identidade é a própria identidade
    Mat3 id = mat3_identity();
    assert(mat3_equal(mat3_inverse(id), id, EPS));

    // matriz de rotação: inversa deve coincidir com a transposta
    Mat3 ry = mat3_rotation_y(1.1);
    Mat3 inv = mat3_inverse(ry);
    Mat3 t = mat3_transpose(ry);
    assert(mat3_equal(inv, t, 1e-9));
    assert(mat3_equal(mat3_mul(ry, inv), id, 1e-9));

    // matriz genérica não-ortogonal, mas invertível
    Mat3 a = {{
        {2, 0, 0},
        {0, 3, 0},
        {0, 0, 4}
    }};
    Mat3 inv_a = mat3_inverse(a);
    assert(mat3_equal(mat3_mul(a, inv_a), id, 1e-9));

    // matriz singular (determinante 0) deve retornar a identidade, sem divisão por zero
    Mat3 singular = {{
        {1, 2, 3},
        {2, 4, 6},
        {1, 1, 1}
    }};
    assert(mat3_equal(mat3_inverse(singular), id, EPS));
}

static void test_mat3_from_quat(void) {
    Mat3 id = mat3_identity();

    // quatérnio identidade deve gerar a matriz identidade
    Quat qid = {1.0, 0.0, 0.0, 0.0};
    assert(mat3_equal(mat3_from_quat(qid), id, 1e-9));

    // quatérnio de rotação de 90 graus em Z deve coincidir com mat3_rotation_z
    Quat q90z = quat_from_axis_angle((Vec3){0.0, 0.0, 1.0}, PI / 2.0);
    Mat3 from_q = mat3_from_quat(q90z);
    Mat3 rz = mat3_rotation_z(PI / 2.0);
    assert(mat3_equal(from_q, rz, 1e-9));

    // mesma checagem para X e Y, cobrindo os três eixos
    Quat q90x = quat_from_axis_angle((Vec3){1.0, 0.0, 0.0}, PI / 2.0);
    assert(mat3_equal(mat3_from_quat(q90x), mat3_rotation_x(PI / 2.0), 1e-9));

    Quat q90y = quat_from_axis_angle((Vec3){0.0, 1.0, 0.0}, PI / 2.0);
    assert(mat3_equal(mat3_from_quat(q90y), mat3_rotation_y(PI / 2.0), 1e-9));

    // matriz gerada a partir de um quatérnio unitário deve ser ortogonal: R * R^T = I
    Quat q = quat_normalize((Quat){0.2, 0.4, -0.3, 0.8});
    Mat3 r = mat3_from_quat(q);
    Mat3 rt = mat3_transpose(r);
    assert(mat3_equal(mat3_mul(r, rt), id, 1e-9));
}

static void test_mat3(void) {
    test_mat3_identity();
    test_mat3_rotations();
    test_mat3_mul_and_mul_vec3();
    test_mat3_transpose();
    test_mat3_inverse();
    test_mat3_from_quat();
    printf("[OK] mat3\n");
}

int main(void) {
    test_vec3();
    test_quat();
    test_mat3();

    printf("Todos os testes passaram com sucesso!\n");
    return 0;
}
