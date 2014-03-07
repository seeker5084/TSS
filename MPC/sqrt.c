#include <stdio.h>
#include <mpc.h>

int main() {
	
	mpc_t z;
	int inex;
	mpc_init2(z, 128);
	mpc_set_ui_ui(z, 0, 1, MPC_RNDNN);

	inex = mpc_sqrt(z, z, MPC_RNDNN);
	mpc_out_str(stdout, 10, 3, z, MPC_RNDNN);
	printf("\n%i %i\n", MPC_INEX_RE(inex), MPC_INEX_IM(inex));

	mpc_clear(z);

}
