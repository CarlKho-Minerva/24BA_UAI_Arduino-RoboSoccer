import sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
clock = time.clock()

# Image center and dimensions
img_center_x = sensor.width() // 2
img_center_y = sensor.height() // 2
img_width = sensor.width()
img_height = sensor.height()

# Center dead zone parameters (adjust as needed)
center_region_x_min = int(img_center_x - img_width * 0.1)
center_region_x_max = int(img_center_x + img_width * 0.1)
center_region_y_min = int(img_center_y - img_height * 0.1)
center_region_y_max = int(img_center_y + img_height * 0.1)

def get_approach_data(x, y):
    """Calculates vector, direction towards the ball (x, y) with dead zone."""
    direction = ""

    # Calculate the direction
    if center_region_x_min <= x <= center_region_x_max and \
       center_region_y_min <= y <= center_region_y_max:
        direction = "C"  # Center
    elif x < center_region_x_min:  # Left
        if y < center_region_y_min:
            direction = "UL"  # Upper Left
        elif y > center_region_y_max:
            direction = "DL"  # Lower Left
        else:
            direction = "L"  # Left
    elif x > center_region_x_max:  # Right
        if y < center_region_y_min:
            direction = "UR"  # Upper Right
        elif y > center_region_y_max:
            direction = "DR"  # Lower Right
        else:
            direction = "R"  # Right
    else:  # Center (horizontally)
        if y < center_region_y_min:
            direction = "U"  # Upper
        elif y > center_region_y_max:
            direction = "D"  # Lower
        else:
            direction = "C" # Center

    # Calculate the vector (no change here)
    if direction == "C":
        dx, dy = 0, 0  # Inside dead zone, no movement
    else:
        dx = x - img_center_x
        dy = y - img_center_y

        # Normalize the vector (optional)
        magnitude = math.sqrt(dx**2 + dy**2)
        if magnitude > 0:
            dx = int(dx / magnitude * 20)
            dy = int(dy / magnitude * 20)

    return dx, dy, direction

while(True):
    clock.tick()
    img = sensor.snapshot()
    img = img.gaussian(1)
    img = img.find_edges(image.EDGE_CANNY, threshold=(30, 70))

    # Visualize the dead zone (optional)
    img.draw_rectangle(center_region_x_min, center_region_y_min,
                       center_region_x_max - center_region_x_min,
                       center_region_y_max - center_region_y_min,
                       color=127, thickness=1)

    # Find circles, but prioritize only one
    largest_circle = None
    for c in img.find_circles(threshold=3500, r_min=5, r_max=40, r_step=2):
        if largest_circle is None or c.r() > largest_circle.r():
            largest_circle = c

    if largest_circle is not None:
        dx, dy, direction = get_approach_data(largest_circle.x(), largest_circle.y())

        print("Ball:", (largest_circle.x(), largest_circle.y()), "dx, dy:", (dx, dy), "Direction:", direction)

        # Visualize the vector (optional)
        if dx != 0 or dy != 0:  # Only draw if there's a vector
            img.draw_line(img_center_x, img_center_y,
                          img_center_x + dx, img_center_y + dy,
                          color=127, thickness=2)

    print("FPS:", clock.fps())
