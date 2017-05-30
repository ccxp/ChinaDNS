#include "iBase64.h"
#include <stdlib.h>
#include <string.h>

static void init_conversion_tables(void);

/*  Global variables used in this source file only */
static unsigned char char_to_base64[256];
static int tables_initialised = 0;

int DecodeBase64(const unsigned char *sSrc, unsigned char *sTarget, const int nSize)
{
    int target_size = 0; /*  Length of target buffer          */
    int nb_block; /*  Total number of block            */
    unsigned char value, /*  Value of Base64 byte             */
    *p_source, /*  Pointer in source buffer         */
    *p_target; /*  Pointer in target buffer         */

    int n;

    if (nSize == 0 || 0 != nSize % 4)
        return (0);

    if (!tables_initialised)
        init_conversion_tables();

    /*  Bit positions
     | byte 1 | byte 2 | byte 3 | byte 4 |
     Encoded block  654321   654321   654321   654321  -> 4 bytes of 6 bits
     | byte 1 | byte 2 | byte 3 |
     Decoded block  65432165 43216543 21654321         -> 3 bytes of 8 bits
     */

    nb_block = nSize / 4;
    target_size = (int) nb_block * 3;
    sTarget[target_size] = '\0';

    p_source = (unsigned char *) sSrc; /*  Point to start of buffers        */
    p_target = sTarget;

    while (nb_block--)
    {
        /*  Byte 1                                                           */
        *p_target = char_to_base64[(unsigned char) *p_source++] << 2;
        value = char_to_base64[(unsigned char) *p_source++];
        *p_target++ += ((value & 0x30) >> 4);

        /*  Byte 2                                                           */
        *p_target = ((value & 0x0F) << 4);
        value = char_to_base64[(unsigned char) *p_source++];
        *p_target++ += ((value & 0x3C) >> 2);

        /*  Byte 3                                                           */
        *p_target = (value & 0x03) << 6;
        value = char_to_base64[(unsigned char) *p_source++];
        *p_target++ += value;
    }

    // 计算末尾有多少个'='
    int equal_sign = 0;
    for (n = nSize - 1; n >= 0 && equal_sign <= 2; --n)
    {
        if (sSrc[n] == '=')
        {
            target_size--;
            equal_sign++;
        }
        else
            break;
    }

    return (target_size);
}

static void init_conversion_tables(void)
{
    if (!tables_initialised)
    {
        unsigned char value, /*  Value to store in table          */
        offset, index; /*  Index in table                   */

        /*  Reset the tables                                                     */
        memset(char_to_base64, 0, sizeof(char_to_base64));

        value = 'A';
        offset = 0;

        for (index = 0; index < 62; index++)
        {
            if (index == 26)
            {
                value = 'a';
                offset = 26;
            }
            else if (index == 52)
            {
                value = '0';
                offset = 52;
            }
            char_to_base64[value + index - offset] = index;
        }
        char_to_base64[(unsigned int) '+'] = 62;
        char_to_base64[(unsigned int) '/'] = 63;

        tables_initialised = 1;

    }
}
