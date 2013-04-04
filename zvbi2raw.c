#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "libzvbi.h"

/**
 * zvbi2raw.c - quick & dirty tool for raw captioning output using libzvbi
 * Cody "codeman38" Boisclair  2013/04/03
 * 
 * Released under the BSD license; see COPYING for details.
 */
 
void print_help();

int main(int argc, char** argv) {
    int line = 21;
    char* device = "/dev/vbi0";
    char parity_mask = 0xff;
    char output_magic = 0;
    
    char c;
    while ((c = getopt(argc, argv, "d:l:ms")) != -1) {
        switch (c) {
            case 'l':
                line = atoi(optarg);
                break;
            case 'd':
                device = optarg;
                break;
            case 's':
                parity_mask = 0x7f;
                break;
            case 'm':
                output_magic = 1;
                break;
            default:
                print_help();
                return;
        }
    }
        
    fprintf(stderr, "Capturing line %d from %s\n", line, device);
    
    /* Prepare the vbi_capture object */
    char* errstr;
    unsigned int service = VBI_SLICED_CAPTION_525;
    vbi_capture* cap =
        vbi_capture_v4l2_new(device, 5, &service, 1, &errstr, 0);
    if (cap == NULL) {
        fprintf(stderr, "Device cannot be opened!\n");
        return 2;
    }
    
    /* Get the vbi_raw_decoder parameters */
    vbi_raw_decoder* dec = vbi_capture_parameters(cap);
    int nrows = dec->count[0] + dec->count[1];
    int buffsize = nrows * dec->bytes_per_line;
    
    /* Output the magic number, if we're going to do that */
    if (output_magic)
        printf("%c%c", 0xff, 0xff); 

    /* Allocate the variables for the vbi_capture_pull call */
    vbi_capture_buffer* buffer_ptr;
    struct timeval timeout;
    vbi_sliced* row_ptr;
    int i;
    
    while (1) {
        vbi_capture_pull_sliced(cap, &buffer_ptr, &timeout);
        for (i = 0; i*sizeof(vbi_sliced) < buffer_ptr->size; i++) {
            row_ptr = ((vbi_sliced*) buffer_ptr->data) + i;
            if (row_ptr->line == line) {
                printf("%c%c", 
                       row_ptr->data[0] & parity_mask,
                       row_ptr->data[1] & parity_mask);
                fflush(stdout);
            }
        }
    }
}

void print_help() {
    printf("zvbi2raw - outputs raw VBI data to stdout using libzvbi\n");
    printf("Options:\n");
    printf("  -l <line>   : Use line <line> of VBI (default: 21)\n");
    printf("  -d <device> : Use <device> as input device (default: /dev/vbi0)\n");
    printf("  -s          : Strip parity bit from output\n");
    printf("  -m          : Output 0xFFFF magic number at beginning of file\n");
    printf("                  (needed for McPoodle's conversion tools)\n");
}
