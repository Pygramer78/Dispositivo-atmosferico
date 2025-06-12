#ifndef MLX_90615
#define MLX_90615
#include <mlx90615.h>
extern MLX90615 mlx;
bool MLX90615_begin(void);
void MLX90615_init(void);
void MLX90615_readObjectTemperature(void);
void MLX90615_readAmbientTemperature(void);
#endif