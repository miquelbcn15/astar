#!usr/bin python3

import gmplot
import os
import webbrowser
import argparse
import sys

# setting the map(latitude, longitude, zoom)
gmap = gmplot.GoogleMapPlotter(41.9333, 2.25, 8)

# input file
in_file = sys.argv[1]

# name of the output file
out_file = in_file.split('.')[-2]+".html"

# saving route
latitude_list  = []
longitude_list = []

with open(in_file, 'r') as f:
    for line in f:
        if line.startswith("#"):
            continue
        fields = line.split('|')
        for field in fields:
            field.strip();
        latitude_list.append(float(fields[1]))
        longitude_list.append(float(fields[2]))

# Add the route
gmap.plot(latitude_list, longitude_list, 'cornflowerblue', edge_width = 6)

# saving the map
gmap.draw(out_file)

# show the map
f = webbrowser.open("file://" + os.path.realpath(out_file))
