#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdlib.h>
#include <stdio.h>

#include "protocolo.h"
#include "rede.h"
 
int main()
{
    int soq = cria_raw_socket("lo");
    struct pacote data;

    rede_escuta(&data, soq);

    double *i = (double *)data.dados;

    printf("%d\n", data.tipo);
    printf("%d\n", data.tamanho);
    printf("%d\n", data.sequencia);
    printf("%f\n", *i);
    printf("%d\n", data.crc);

    return EXIT_SUCCESS;
}
