/*
* Copyright (c) 2019-2025 Allwinner Technology Co., Ltd. ALL rights reserved.
*
* Allwinner is a trademark of Allwinner Technology Co.,Ltd., registered in
* the the people's Republic of China and other countries.
* All Allwinner Technology Co.,Ltd. trademarks are used with permission.
*
* DISCLAIMER
* THIRD PARTY LICENCES MAY BE REQUIRED TO IMPLEMENT THE SOLUTION/PRODUCT.
* IF YOU NEED TO INTEGRATE THIRD PARTY'S TECHNOLOGY (SONY, DTS, DOLBY, AVS OR MPEGLA, ETC.)
* IN ALLWINNERS'SDK OR PRODUCTS, YOU SHALL BE SOLELY RESPONSIBLE TO OBTAIN
* ALL APPROPRIATELY REQUIRED THIRD PARTY LICENCES.
* ALLWINNER SHALL HAVE NO WARRANTY, INDEMNITY OR OTHER OBLIGATIONS WITH RESPECT TO MATTERS
* COVERED UNDER ANY REQUIRED THIRD PARTY LICENSE.
* YOU ARE SOLELY RESPONSIBLE FOR YOUR USAGE OF THIRD PARTY'S TECHNOLOGY.
*
*
* THIS SOFTWARE IS PROVIDED BY ALLWINNER"AS IS" AND TO THE MAXIMUM EXTENT
* PERMITTED BY LAW, ALLWINNER EXPRESSLY DISCLAIMS ALL WARRANTIES OF ANY KIND,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING WITHOUT LIMITATION REGARDING
* THE TITLE, NON-INFRINGEMENT, ACCURACY, CONDITION, COMPLETENESS, PERFORMANCE
* OR MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL ALLWINNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS, OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <hal_log.h>
#include <hal_cmd.h>
#include <sunxi_hal_twi.h>

#define TEST_READ 0
#define TEST_WRITE 1

static int cmd_test_twi(int argc, const char **argv)
{
    twi_msg_t msg;
    twi_port_t port;
    uint16_t addr;
    char reg_addr, reg_val = 0, rw = TEST_READ;
    int c;

    if (argc < 5)
	{
        hal_log_info("Usage:");
        hal_log_info("\ttwi [port] [slave_addr] [reg] -r");
        hal_log_info("\t                              -w [val]");
        return -1;
    }

	hal_log_info("Run twi test");

    port = strtol(argv[1], NULL, 0);
    addr = strtol(argv[2], NULL, 0);
    reg_addr = strtol(argv[3], NULL, 0);

    while ((c = getopt(argc, (char *const *)argv, "r:w")) != -1)
	{
        switch (c)
        {
            case 'r':
				hal_log_info("twi read test");
                rw = TEST_READ;
                break;
            case 'w':
				hal_log_info("twi write test");
                rw = TEST_WRITE;
                reg_val = strtol(argv[5], NULL, 0);
                break;
			default:
				hal_log_err("invalid param!");
        }
    }

    hal_twi_init(port);
    hal_twi_control(port, I2C_SLAVE, &addr);

    if (rw == TEST_READ)
    {
        hal_twi_read(port, reg_addr, &reg_val, 1);
        hal_log_info("reg_val: 0x%x", reg_val);
    }
    else if (rw == TEST_WRITE)
    {
        /*
         * hal_twi_write bug workaround
         */
        uint8_t buf[2];

        buf[0] = reg_addr;
        buf[1] = reg_val;
        msg.flags = 0;
        msg.addr =  addr;
        msg.len = 2;
        msg.buf = buf;

        hal_twi_control(port, I2C_RDWR, &msg);
    }

	hal_log_info("Twi test finish");

    hal_twi_uninit(port);
	
    return 0;
}

FINSH_FUNCTION_EXPORT_CMD(cmd_test_twi, hal_twi, twi hal APIs tests)
