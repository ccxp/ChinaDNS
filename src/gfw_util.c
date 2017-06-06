#include "gfw_util.h"
#include "iBase64.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*********************************************/

static const int MAX_POS = 16;
static int m_pos[16];

static char* m_buf1 = NULL;
static char* m_buf2 = NULL;
static char* m_buf3 = NULL;
static int m_buf2_pos = 0;
static const int m_buf_size = 10240;

typedef struct
{
    short dotcnt;
    short type;
    const char* host;
} stGFWInfo;

static const int mem_buf_size = 10240;
static char** mem_buf_list = NULL;
static int mem_buf_count = 0;

static char* mem_ptr = NULL;
static int mem_pos = 10240;

static const int gfw_info_item_alloc = 2048;
static stGFWInfo* gfw_info_list = NULL;
static int gfw_info_count = 0;
static int gfw_info_alloc_count = 0;

static const char* google_hosts =
        "google.com\ngoogle.ad\ngoogle.ae\ngoogle.com.af\ngoogle.com.ag\ngoogle.com.ai\ngoogle.al\ngoogle.am\ngoogle.co.ao\ngoogle.com.ar\ngoogle.as\ngoogle.at\ngoogle.com.au\ngoogle.az\ngoogle.ba\ngoogle.com.bd\ngoogle.be\ngoogle.bf\ngoogle.bg\ngoogle.com.bh\ngoogle.bi\ngoogle.bj\ngoogle.com.bn\ngoogle.com.bo\ngoogle.com.br\ngoogle.bs\ngoogle.bt\ngoogle.co.bw\ngoogle.by\ngoogle.com.bz\ngoogle.ca\ngoogle.cd\ngoogle.cf\ngoogle.cg\ngoogle.ch\ngoogle.ci\ngoogle.co.ck\ngoogle.cl\ngoogle.cm\ngoogle.cn\ngoogle.com.co\ngoogle.co.cr\ngoogle.com.cu\ngoogle.cv\ngoogle.com.cy\ngoogle.cz\ngoogle.de\ngoogle.dj\ngoogle.dk\ngoogle.dm\ngoogle.com.do\ngoogle.dz\ngoogle.com.ec\ngoogle.ee\ngoogle.com.eg\ngoogle.es\ngoogle.com.et\ngoogle.fi\ngoogle.com.fj\ngoogle.fm\ngoogle.fr\ngoogle.ga\ngoogle.ge\ngoogle.gg\ngoogle.com.gh\ngoogle.com.gi\ngoogle.gl\ngoogle.gm\ngoogle.gp\ngoogle.gr\ngoogle.com.gt\ngoogle.gy\ngoogle.com.hk\ngoogle.hn\ngoogle.hr\ngoogle.ht\ngoogle.hu\ngoogle.co.id\ngoogle.ie\ngoogle.co.il\ngoogle.im\ngoogle.co.in\ngoogle.iq\ngoogle.is\ngoogle.it\ngoogle.je\ngoogle.com.jm\ngoogle.jo\ngoogle.co.jp\ngoogle.co.ke\ngoogle.com.kh\ngoogle.ki\ngoogle.kg\ngoogle.co.kr\ngoogle.com.kw\ngoogle.kz\ngoogle.la\ngoogle.com.lb\ngoogle.li\ngoogle.lk\ngoogle.co.ls\ngoogle.lt\ngoogle.lu\ngoogle.lv\ngoogle.com.ly\ngoogle.co.ma\ngoogle.md\ngoogle.me\ngoogle.mg\ngoogle.mk\ngoogle.ml\ngoogle.com.mm\ngoogle.mn\ngoogle.ms\ngoogle.com.mt\ngoogle.mu\ngoogle.mv\ngoogle.mw\ngoogle.com.mx\ngoogle.com.my\ngoogle.co.mz\ngoogle.com.na\ngoogle.com.nf\ngoogle.com.ng\ngoogle.com.ni\ngoogle.ne\ngoogle.nl\ngoogle.no\ngoogle.com.np\ngoogle.nr\ngoogle.nu\ngoogle.co.nz\ngoogle.com.om\ngoogle.com.pa\ngoogle.com.pe\ngoogle.com.pg\ngoogle.com.ph\ngoogle.com.pk\ngoogle.pl\ngoogle.pn\ngoogle.com.pr\ngoogle.ps\ngoogle.pt\ngoogle.com.py\ngoogle.com.qa\ngoogle.ro\ngoogle.ru\ngoogle.rw\ngoogle.com.sa\ngoogle.com.sb\ngoogle.sc\ngoogle.se\ngoogle.com.sg\ngoogle.sh\ngoogle.si\ngoogle.sk\ngoogle.com.sl\ngoogle.sn\ngoogle.so\ngoogle.sm\ngoogle.sr\ngoogle.st\ngoogle.com.sv\ngoogle.td\ngoogle.tg\ngoogle.co.th\ngoogle.com.tj\ngoogle.tk\ngoogle.tl\ngoogle.tm\ngoogle.tn\ngoogle.to\ngoogle.com.tr\ngoogle.tt\ngoogle.com.tw\ngoogle.co.tz\ngoogle.com.ua\ngoogle.co.ug\ngoogle.co.uk\ngoogle.com.uy\ngoogle.co.uz\ngoogle.com.vc\ngoogle.co.ve\ngoogle.vg\ngoogle.co.vi\ngoogle.com.vn\ngoogle.vu\ngoogle.ws\ngoogle.rs\ngoogle.co.za\ngoogle.co.zm\ngoogle.co.zw\ngoogle.cat\n";
static const char* blogspot_hosts =
        "blogspot.ca\nblogspot.co.uk\nblogspot.com\nblogspot.com.ar\nblogspot.com.au\nblogspot.com.br\nblogspot.com.by\nblogspot.com.co\nblogspot.com.cy\nblogspot.com.ee\nblogspot.com.eg\nblogspot.com.es\nblogspot.com.mt\nblogspot.com.ng\nblogspot.com.tr\nblogspot.com.uy\nblogspot.de\nblogspot.gr\nblogspot.in\nblogspot.mx\nblogspot.ch\nblogspot.fr\nblogspot.ie\nblogspot.it\nblogspot.pt\nblogspot.ro\nblogspot.sg\nblogspot.be\nblogspot.no\nblogspot.se\nblogspot.jp\nblogspot.in\nblogspot.ae\nblogspot.al\nblogspot.am\nblogspot.ba\nblogspot.bg\nblogspot.ch\nblogspot.cl\nblogspot.cz\nblogspot.dk\nblogspot.fi\nblogspot.gr\nblogspot.hk\nblogspot.hr\nblogspot.hu\nblogspot.ie\nblogspot.is\nblogspot.kr\nblogspot.li\nblogspot.lt\nblogspot.lu\nblogspot.md\nblogspot.mk\nblogspot.my\nblogspot.nl\nblogspot.no\nblogspot.pe\nblogspot.qa\nblogspot.ro\nblogspot.ru\nblogspot.se\nblogspot.sg\nblogspot.si\nblogspot.sk\nblogspot.sn\nblogspot.tw\nblogspot.ug\nblogspot.cat\n";

/*********************************************/
static int get_dot_pos(const char* line, int* p, int pLen)
{
    int cnt = 0, l = strlen(line);
    int i = 0;
    for (; i < l; i++)
    {
        if (line[i] == '.')
        {
            if (cnt < pLen)
                p[cnt] = i;
            cnt++;
        }
    }
    return cnt;
}

/*********************************************/

static void handleLine(char* line)
{
    int type = 1, dotcnt, len = strlen(line);
    char* p;
    stGFWInfo* ptr;
    
    if (0 == len)
        return;
    if (line[0] == '!' || line[0] == '[')
        return;

    if (line[0] == '@')
        type = 2;

    while (len)
    {
        if (line[0] == '@' || line[0] == '|')
        {
            line++;
            len--;
            continue;
        }
        break;
    }

    if (len >= 7 && memcmp(line, "http://", 7) == 0)
    {
        line += 7;
        len -= 7;
    }

    if (len >= 8 && memcmp(line, "https://", 8) == 0)
    {
        line += 8;
        len -= 8;
    }

    p = strstr(line, "/");
    if (p)
    {
        *p = '\0';
        len = (p - line);
    }
    p = strstr(line, "*.");
    if (p)
    {
        line = p + 2;
        len -= 2;
    }
    if (len && line[0] == '.')
    {
        line++;
        len--;
    }

    if (0 == len)
        return;
    if (strstr(line, "*"))
        return;

    //printf("%d %s\n", type, line);

    dotcnt = get_dot_pos(line, NULL, 0);
    if (dotcnt > MAX_POS || 0 == dotcnt)
        return;
    //printf("%d %d\n", type, dotcnt);

    if (gfw_info_count >= gfw_info_alloc_count)
    {
        gfw_info_alloc_count += gfw_info_item_alloc;
        gfw_info_list = realloc(gfw_info_list, sizeof(stGFWInfo) * gfw_info_alloc_count);
    }
    ptr = gfw_info_list + gfw_info_count;
    gfw_info_count++;

    len = strlen(line) + 1;
    if (mem_pos + len > mem_buf_size)
    {
        mem_buf_count++;
        mem_buf_list = (char**) realloc(mem_buf_list, sizeof(char*) * mem_buf_count);
        mem_buf_list[mem_buf_count - 1] = mem_ptr = (char*) malloc(mem_buf_size);
        mem_pos = 0;
    }

    ptr->dotcnt = dotcnt;
    ptr->type = type;
    ptr->host = mem_ptr + mem_pos;
    memcpy(mem_ptr + mem_pos, line, len);
    mem_pos += len;
}

static void handleStaticHosts(const char* hosts)
{
    int pos1 = 0, pos2 = 0, len = strlen(hosts);
    for (pos2 = 0; pos2 < len; pos2++)
    {
        if (hosts[pos2] == '\n' || hosts[pos2] == '\r')
        {
            memcpy(m_buf2, hosts + pos1, pos2 - pos1);
            m_buf2[pos2 - pos1] = '\0';

            handleLine(m_buf2);

            pos1 = pos2 + 1;
        }
    }
}
static void handleBase64Line(char* line)
{
    int pos1 = 0, pos2 = 0;
    int cnt = DecodeBase64(line, m_buf2 + m_buf2_pos, strlen(line));
    m_buf2_pos += cnt;
    m_buf2[m_buf2_pos] = '\0';

    for (pos2 = 0; pos2 < m_buf2_pos; pos2++)
    {
        if (m_buf2[pos2] == '\n' || m_buf2[pos2] == '\r')
        {
            m_buf2[pos2] = '\0';

            handleLine(m_buf2 + pos1);
            pos1 = pos2 + 1;
        }
    }
    if (pos1 < m_buf2_pos && pos1 > 0)
    {
    	if (pos1 > m_buf2_pos - pos1)
    		memcpy(m_buf2, m_buf2 + pos1, m_buf2_pos - pos1);
    	else
    	{
    		memcpy(m_buf3, m_buf2 + pos1, m_buf2_pos - pos1);
    		memcpy(m_buf2, m_buf3, m_buf2_pos - pos1);
    	}
        
        m_buf2_pos = m_buf2_pos - pos1;
        m_buf2[m_buf2_pos] = '\0';
    }
}

/*********************************************/

static int ReadConfig(FILE* fp)
{
    char* buf = m_buf1;
    int cnt, flg = 1;

    char* str = buf;
    int pos1, pos2;
    while (flg)
    {
        cnt = fread(str, 1, m_buf_size - (str - buf) - 1, fp);
        str[cnt] = '\0';

        flg = cnt > 0 ? 1 : 0;
        if (flg)
        {
            pos1 = 0;
            pos2 = 0;
            cnt += (str - buf);
            for (pos2 = 0; pos2 < cnt; pos2++)
            {

                if (buf[pos2] == '\n' || buf[pos2] == '\r')
                {
                    buf[pos2] = '\0';

                    handleBase64Line(buf + pos1);

                    pos1 = pos2 + 1;
                }
            }
            if (pos1 < cnt)
            {
                if (pos1 > 0)
                    memcpy(buf, buf + pos1, cnt - pos1);
                str = buf + cnt - pos1;
                *str = '\0';
            }
            else
                str = buf;
        }
        else if (str > buf)
        {
            handleBase64Line(buf);
        }
    }

    return 0;
}

/*********************************************/

static int cmp_gfw_info(const void *a, const void *b)
{
    stGFWInfo *l = (stGFWInfo *) a;
    stGFWInfo *r = (stGFWInfo *) b;
    if (l->dotcnt == r->dotcnt)
        return strcmp(l->host, r->host);
    return l->dotcnt - r->dotcnt;
}

/*********************************************/

int LoadGFWData(const char* conf)
{
    FILE* fp = fopen(conf, "r");
    if (NULL == fp)
    {
        printf("read open %s fail\n", conf);
        return -1;
    }

    m_buf1 = (char*) malloc(m_buf_size);
    m_buf2 = (char*) malloc(m_buf_size);
    m_buf3 = (char*) malloc(m_buf_size);
    ReadConfig(fp);
    fclose(fp);

    if (m_buf2_pos)
        handleLine(m_buf2);

    handleStaticHosts(google_hosts);
    handleStaticHosts(blogspot_hosts);

    free(m_buf1);
    free(m_buf2);
    free(m_buf3);

    qsort(gfw_info_list, gfw_info_count, sizeof(stGFWInfo), cmp_gfw_info);

    return 0;
}

int GetGFWType(const char* host)
{
    if (0 == gfw_info_count)
        return GFW_TYPE_UNKNOWN;

    stGFWInfo key;
    stGFWInfo* r;
    int dotcnt, i = 0;

    dotcnt = get_dot_pos(host, m_pos, MAX_POS);
    if (dotcnt > MAX_POS)
        return GFW_TYPE_UNKNOWN;

    key.dotcnt = dotcnt;
    key.host = host;
    r = (stGFWInfo*) bsearch(&key, gfw_info_list, gfw_info_count, sizeof(stGFWInfo), cmp_gfw_info);
    if (r)
        return r->type;

    for (i = 0; i < dotcnt - 1; i++)
    {
        key.dotcnt = dotcnt - i - 1;
        key.host = host + m_pos[i] + 1;

        r = (stGFWInfo*) bsearch(&key, gfw_info_list, gfw_info_count, sizeof(stGFWInfo), cmp_gfw_info);
        if (r)
            return r->type;
    }

    return GFW_TYPE_UNKNOWN;
}

/*
int main(int argc, char** argv)
{
    int ret = LoadGFWData("./gfw.list");
    printf("LoadData ret=%d\n", ret);

    printf("%s=%d\n", argv[1], GetGFWType(argv[1]));
    return 0;
}*/


