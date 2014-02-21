from java.awt import Color
import math

f = 255 / 500.
for node in g.nodes:
    node.x = int(node.x_coor)
    node.y = int(node.y_coor)
    r = math.trunc(int(node.x_coor)*f)
    b = math.trunc(int(node.y_coor)*f)
    node.color = Color(r, 0, b)