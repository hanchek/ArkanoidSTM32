#!/usr/bin/env python3
"""
SSD1306 Image Converter for Page Addressing Mode
Converts bitmap images to C++ arrays for SSD1306 OLED display in page addressing mode.
"""

import sys
from pathlib import Path
from PIL import Image

# ========== CONFIGURATION ==========
# Change these variables to configure the conversion

INPUT_IMAGE = "gameover.bmp"             # Path to the input image file
OUTPUT_FILE = "GameOver.h"            # Output file path (set to None to print to stdout)
ARRAY_NAME = "GameOver"            # Name of the C++ array
RESIZE_TO = None                    # Resize image to (width, height) or None to use original size
                                    # Example: RESIZE_TO = (128, 64) for full SSD1306 display
THRESHOLD = 128                     # Brightness threshold 0-255 (pixels >= threshold will be lit)
INVERT = False                      # Invert colors (True = swap black/white)
BYTES_PER_LINE = 16                 # Number of bytes per line in the output array

# ===================================


def convert_image_to_page_array(image_path, resize_to=None, threshold=128, invert=False):
    """Convert an image to SSD1306 page addressing mode byte array."""
    try:
        img = Image.open(image_path)
        
        if resize_to:
            img = img.resize(resize_to, Image.Resampling.LANCZOS)
        
        # Convert to grayscale
        img = img.convert('L')
        width, height = img.size
        pixels = img.load()
        
        buffer_size = width * (height // 8)
        buffer = [0] * buffer_size

        for page in range(height // 8):
            for x in range(width):
                byte_value = 0
                for bit in range(8):
                    y = page * 8 + bit

                    # Revert y axis
                    pixel_value = pixels[x, height - 1 - y]
                    is_lit = pixel_value >= threshold
                    
                    if invert:
                        is_lit = not is_lit
                    
                    if is_lit:
                        byte_value |= (1 << bit)
                
                buffer_index = page * width + x
                buffer[buffer_index] = byte_value
        
        return buffer, width, height
        
    except FileNotFoundError:
        print(f"Error: File '{image_path}' not found.", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error processing image: {e}", file=sys.stderr)
        sys.exit(1)


def generate_cpp_array(buffer, width, height, array_name="ImageData", bytes_per_line=16):
    lines = []
    lines.append("#include <array>")
    lines.append("#include <cstdint>")
    lines.append("")
    lines.append(f"// Image dimensions: {width}x{height} pixels")
    lines.append(f"// Buffer size: {len(buffer)} bytes")
    lines.append(f"constexpr std::array<uint8_t, {len(buffer)}> {array_name} = {{")
    
    for i in range(0, len(buffer), bytes_per_line):
        chunk = buffer[i:i + bytes_per_line]
        hex_values = [f"0x{byte:02X}" for byte in chunk]
        line = "    " + ", ".join(hex_values)
        
        if i + bytes_per_line < len(buffer):
            line += ","
        
        lines.append(line)
    
    lines.append("};")
    lines.append("")
    
    return "\n".join(lines)


def main():
    script_dir = Path(__file__).parent
    input_path = script_dir / INPUT_IMAGE
    output_path = script_dir / OUTPUT_FILE if OUTPUT_FILE else None
    
    if not 0 <= THRESHOLD <= 255:
        print(f"Error: Threshold must be between 0 and 255 (got {THRESHOLD})", file=sys.stderr)
        sys.exit(1)
    
    if RESIZE_TO is not None:
        if len(RESIZE_TO) != 2:
            print(f"Error: RESIZE_TO must be a tuple of (width, height)", file=sys.stderr)
            sys.exit(1)
        if RESIZE_TO[1] % 8 != 0:
            print(f"Error: Height must be a multiple of 8 (got {RESIZE_TO[1]})", file=sys.stderr)
            sys.exit(1)
    
    print(f"Converting '{input_path}'...", file=sys.stderr)
    if RESIZE_TO:
        print(f"Resizing to: {RESIZE_TO[0]}x{RESIZE_TO[1]}", file=sys.stderr)
    else:
        print(f"Using original image size", file=sys.stderr)
    print(f"Threshold: {THRESHOLD}", file=sys.stderr)
    print(f"Invert: {INVERT}", file=sys.stderr)
    
    buffer, width, height = convert_image_to_page_array(
        str(input_path),
        resize_to=RESIZE_TO,
        threshold=THRESHOLD,
        invert=INVERT
    )
    
    if height % 8 != 0:
        print(f"Error: Image height must be a multiple of 8 (got {height})", file=sys.stderr)
        print(f"Please resize your image or use RESIZE_TO parameter", file=sys.stderr)
        sys.exit(1)
    
    print(f"Final image size: {width}x{height} pixels", file=sys.stderr)
    
    cpp_code = generate_cpp_array(buffer, width, height, ARRAY_NAME, BYTES_PER_LINE)
    
    if output_path:
        with open(output_path, 'w') as f:
            f.write(cpp_code)
        print(f"Output written to '{output_path}'", file=sys.stderr)
        print(f"Array size: {len(buffer)} bytes", file=sys.stderr)
    else:
        print(cpp_code)
    
    print("Done!", file=sys.stderr)


if __name__ == "__main__":
    main()

