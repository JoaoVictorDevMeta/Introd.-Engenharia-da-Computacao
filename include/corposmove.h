// Documento header para definir as structs e funções relativas
// ao movimento dos corpos rígidos.

typedef struct {
    double comprimento;
    double altura;
    double massa;
    double momentOfInertia;
} BoxShape;

// Calculates the inertia of a box shape and stores it in the
// momentOfInertia variable.
void CalculateBoxInertia(BoxShape boxShape) {
    double m = boxShape.massa;
    double w = boxShape.comprimento;
    double h = boxShape.altura;
    boxShape.momentOfInertia = m * (w * w + h * h) / 12;
}