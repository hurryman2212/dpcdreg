#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/fcntl.h>
#include <sys/unistd.h>

int main(int argc, char *argv[]) {
  /* Parse arguments. */
  if (argc != 4) {
    fprintf(stderr, "usage: %s DRI_DP_PATH ADDRESS_HEX BYTES_DEC\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  const char *dri_dp_path = argv[1];
  uint32_t dpcd_addr = strtoul(argv[2], NULL, 16);
  uint32_t dpcd_size = atoi(argv[3]);

  /* Print parsed values. */
  fprintf(stderr, "debug: dri_dp_path <- %s\n", dri_dp_path);
  fprintf(stderr, "debug: dpcd_addr <- 0x%X\n", dpcd_addr);
  fprintf(stderr, "debug: dpcd_size <- %u\n", dpcd_size);

  /* Open directory. */
  int dirfd = open(dri_dp_path, O_DIRECTORY | O_RDONLY);
  if (dirfd == -1) {
    perror("dirfd: open");
    exit(EXIT_FAILURE);
  }

  /* Open files. */
  int dpcd_addr_fd = openat(dirfd, "aux_dpcd_address", O_RDWR);
  if (dpcd_addr_fd == -1) {
    perror("dpcd_addr_fd: open");
    exit(EXIT_FAILURE);
  }
  int dpcd_size_fd = openat(dirfd, "aux_dpcd_size", O_RDWR); // ?
  if (dpcd_size_fd == -1) {
    perror("dpcd_size_fd: open");
    exit(EXIT_FAILURE);
  }
  int dpcd_data_fd = openat(dirfd, "aux_dpcd_data", O_RDONLY);
  if (dpcd_data_fd == -1) {
    perror("dpcd_data_fd: open");
    exit(EXIT_FAILURE);
  }

  /* Set address to read. */
  if (write(dpcd_addr_fd, &dpcd_addr, sizeof(dpcd_addr)) == -1) {
    perror("dpcd_addr_fd: write");
    exit(EXIT_FAILURE);
  }

  /* Set size to read. */
  if (write(dpcd_size_fd, &dpcd_size, sizeof(dpcd_size)) == -1) {
    perror("dpcd_size_fd: write");
    exit(EXIT_FAILURE);
  }

  /* Setup data buffer. */
  uint8_t *buf = calloc(dpcd_size, 1);
  if (!buf) {
    perror("calloc");
    exit(EXIT_FAILURE);
  }

  /* Read data. */
  if (read(dpcd_data_fd, buf, dpcd_size) == -1) {
    perror("dpcd_size_fd: read");
    exit(EXIT_FAILURE);
  }

  /* Print the result. */
  for (uint32_t i = 0; i < dpcd_size; ++i)
    printf("buf[%u] = 0x%hhX\n", i, buf[i]);

  return EXIT_SUCCESS;
}
