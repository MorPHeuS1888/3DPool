#pragma once

class Renderable {
public:
    virtual void render() = 0; // Método virtual puro para renderizar o objeto
    virtual ~Renderable() = default; // Destrutor virtual padrão
};
