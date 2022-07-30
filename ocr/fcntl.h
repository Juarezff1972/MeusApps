
   /* File access modes used with open() and fcntl() */
#  define O_RDONLY	0000000   /* Open for reading only */
#  define O_WRONLY	0000001   /* Open for writing only */
#  define O_RDWR	0000002   /* Open for reading or writing */
#  define O_ACCMODE	0000003   /* Mask for file access modes */

   /* Values for oflag used by open() */
   /* Values of these must match corresponding F* values in <sys/file.h>. */
#  define O_CREAT   0000400   /* Open with file create (uses third open arg) */
#  define O_TRUNC   0001000   /* Open with truncation */
#  define O_EXCL    0002000   /* Refuse to create if it already exists */
