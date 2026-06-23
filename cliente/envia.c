#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>

#include <protocolo.h>
#include <rede.h>

int main()
{
    int soq = cria_raw_socket("enp3s0");
    struct pacote data;
    double i = 500.71;
    if(constroi_pacote(&data, sizeof(i), TIPO_DADOS, (uint8_t *) &i))
        fprintf(stderr, "Erro ao construir pacote");

    rede_envia(&data, soq);

    close(soq);

    return EXIT_SUCCESS;
}
