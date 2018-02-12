#include <assert.h>

#include "teh_model.h"
#include "bspc.h"

struct teh_model in;
struct teh_model out;

int main(int argc, char *argv[])
{
	assert (argc == 2);
	read_teh_model_vertices(&in, argv[1]);
	assert (0 < in.triangle_c);
	assert (0 < in.frame_c);

	free_teh_model(&in);
	return 0;
}
