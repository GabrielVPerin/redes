from PIL import Image

# CONFIG
IMAGE_PATH = "map.png"
TOL = 20  # tolerância de cor

# defina suas 3 cores aqui
COLORS = {
    (0, 0, 0): 1,        # preto -> parede
    (255, 255, 255): 0,  # branco -> vazio
    (255, 126, 0): 3     # amarelo -> player (exemplo)
}

def match_color(pixel):
    for color, value in COLORS.items():
        if all(abs(pixel[i] - color[i]) <= TOL for i in range(3)):
            return value
    return 0  # fallback

img = Image.open(IMAGE_PATH).convert("RGB")
width, height = img.size

matrix = []

for y in range(height):
    row = []
    for x in range(width):
        pixel = img.getpixel((x, y))
        row.append(match_color(pixel))
    matrix.append(row)

# print como matriz genérica (Python-style)
for row in matrix:
    print(row)
