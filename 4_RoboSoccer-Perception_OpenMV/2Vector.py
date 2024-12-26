import sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
clock = time.clock()

# Image center
img_center_x = sensor.width() // 2
img_center_y = sensor.height() // 2

def get_approach_vector(x, y):
    """Calculates a vector towards the ball (x, y)."""
    dx = x - img_center_x
    dy = y - img_center_y

    # Normalize the vector (optional, for consistent speed)
    magnitude = math.sqrt(dx**2 + dy**2)
    if magnitude > 0:  # Avoid division by zero
        dx = int(dx / magnitude * 20) # you can multply by a gain here
        dy = int(dy / magnitude * 20)

    return dx, dy

while(True):
    clock.tick()
    img = sensor.snapshot()
    img = img.gaussian(1)
    img = img.find_edges(image.EDGE_CANNY, threshold=(30, 70))

    for c in img.find_circles(threshold=3000, r_min=5, r_max=40, r_step=2):
        dx, dy = get_approach_vector(c.x(), c.y())
        print("Ball:", (c.x(), c.y()), "Vector:", (dx, dy))

        # Visualize the vector (optional)
        img.draw_line(img_center_x, img_center_y,
                      img_center_x + dx, img_center_y + dy,
                      color=127, thickness=2)

    print("FPS:", clock.fps())
