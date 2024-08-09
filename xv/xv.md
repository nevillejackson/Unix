xvpng.c

1. #include <zlib.h>

2. setjmp(png_ptr->jmpbuf)
change to
   setjmp(png_jmpbuf(png_ptr))

3. The png_info struct was removed from the png.h in 1.5.0 and now you should use this pointer with the png_get_* and png_set_* functions.

The png_info structure is designed to provide information about the PNG file. At one time, the fields of png_info were intended to be directly accessible to the user. However, this tended to cause problems with applications using dynamically loaded libraries, and as a result a set of interface functions for png_info (the png_get_*() and png_set_*() functions) was developed, and direct access to the png_info fields was deprecated..

For example:

png_uint_32 height;
height = png_get_image_height( png_ptr,  info_ptr);

If you are trying to set contents of info_ptr which is no longer directly accessible, use png_set_IHDR()


http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-4.3

You now need to fill in the png_info structure with all the data you wish to write before the actual image. Note that the only thing you are allowed to write after the image is the text chunks and the time chunk (as of PNG Specification 1.2, anyway). See png_write_end() and the latest PNG specification for more information on that. If you wish to write them before the image, fill them in now, and flag that data as being valid. If you want to wait until after the data, don't fill them until png_write_end(). For all the fields in png_info and their data types, see png.h. For explanations of what the fields contain, see the PNG specification.

Some of the more important parts of the png_info are:

    png_set_IHDR(png_ptr, info_ptr, width, height,
       bit_depth, color_type, interlace_type,
       compression_type, filter_method)
    width          - holds the width of the image
                     in pixels (up to 2^31).
    height         - holds the height of the image
                     in pixels (up to 2^31).
    bit_depth      - holds the bit depth of one of the
                     image channels.
                     (valid values are 1, 2, 4, 8, 16
                     and depend also on the
                     color_type.  See also significant
                     bits (sBIT) below).
    color_type     - describes which color/alpha
                     channels are present.
                     PNG_COLOR_TYPE_GRAY
                        (bit depths 1, 2, 4, 8, 16)
                     PNG_COLOR_TYPE_GRAY_ALPHA
                        (bit depths 8, 16)
                     PNG_COLOR_TYPE_PALETTE
                        (bit depths 1, 2, 4, 8)
                     PNG_COLOR_TYPE_RGB
                        (bit_depths 8, 16)
                     PNG_COLOR_TYPE_RGB_ALPHA
                        (bit_depths 8, 16)

                     PNG_COLOR_MASK_PALETTE
                     PNG_COLOR_MASK_COLOR
                     PNG_COLOR_MASK_ALPHA
......   see man page for full list

