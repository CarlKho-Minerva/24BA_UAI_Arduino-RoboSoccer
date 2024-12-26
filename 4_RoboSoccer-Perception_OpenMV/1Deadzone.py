import sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
clock = time.clock()

# Image dimensions
img_width = sensor.width()
img_height = sensor.height()

# Bounding box cross parameters (adjust percentages as needed)
h_third = img_width // 3
v_third = img_height // 3

def get_9_direction(x, y):
    """Determines the 9-region direction of the ball (x, y)."""
    h_region = ""
    v_region = ""

    if x < h_third:
        h_region = "L"
    elif x < 2 * h_third:
        h_region = "C"
    else:
        h_region = "R"

    if y < v_third:
        v_region = "U"
    elif y < 2 * v_third:
        v_region = ""  # No vertical prefix for the middle row
    else:
        v_region = "D"

    return v_region + h_region

while(True):
    clock.tick()
    img = sensor.snapshot()
    img = img.gaussian(1)
    img = img.find_edges(image.EDGE_CANNY, threshold=(30, 70))

    # Draw bounding box cross for visualization
    img.draw_line(h_third, 0, h_third, img_height, color=127, thickness=1)
    img.draw_line(2 * h_third, 0, 2 * h_third, img_height, color=127, thickness=1)
    img.draw_line(0, v_third, img_width, v_third, color=127, thickness=1)
    img.draw_line(0, 2 * v_third, img_width, 2 * v_third, color=127, thickness=1)

    for c in img.find_circles(threshold=3000, r_min=5, r_max=40, r_step=2):
        direction = get_9_direction(c.x(), c.y())
        print("Ball:", (c.x(), c.y()), "Direction:", direction)

    print("FPS:", clock.fps())
