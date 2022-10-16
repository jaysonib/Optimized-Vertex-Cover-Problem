#!/usr/bin/env python3
import sys
import re

street_names = {}

# floating point format
def floating_point(val):
	return float("{0:.2f}".format(val[0])), float("{0:.2f}".format(val[1]))

# convert string into tuple
def convert_tuple(str):
	tuple1 = None
	try:
		str = str[1:-1]
		tuple1 = tuple(map(int, str.split(',')))
	except:
		print("Error: An exception occured")
	return tuple1

# merge spaced words and remove "" from the street name
def edit_street(words):
	try:
		if words[1][-1] != '"':
			for i in range(len(words)):
				words[1] = words[1] + ' ' + words[2]
				if words[2][-1] == '"':
					words.remove(words[2])
					break
				words.remove(words[2])
		words[1] = words[1][1:-1]
	except:
		print("Error: an error occured in street name")
	return words[1]

# check for the same co-ordinates
def same_coordinates(words):
	temp = []
	try:
		for i in range(len(words)-2):
			temp1 = convert_tuple(words[i+2])
			if temp1 in temp:
				return None
			temp.append(temp1)
	except:
		print("Error: an error occured in the co-ordinates")
	return temp

# add the street name
def add(words):
	words[1] = edit_street(words)
	if words[1].lower() in street_names.keys():
		print('Error: add specified for a street already exist')
	else:
		try:
			a = same_coordinates(words)
			if a:
				street_names[words[1].lower()] = list(a)
			else:
				print('Error: add specified co-ordinates are the same')
		except:
			print("Error: error occured in adding the street")


# modify the street co-ordinates
def mod(words):
	words[1] = edit_street(words)
	if words[1].lower() not in street_names.keys():
		print('Error: mod specifiedfor a street does not exist')
	else:
		try:
			a = same_coordinates(words)
			if a:
				street_names[words[1].lower()] = list(a)
			else:
				print("Error: We can't modify the street co-ordinates, because the co-ordinates are the same")
		except:
			print("Error: an error occured during modifying the street")

# remove the street name from the dictionary
def rm(words):
	words[1] = edit_street(words)
	try:
		if words[1].lower() not in street_names.keys():
			print('Error: rm street does not exists')
		else:
			street_names.pop(words[1].lower(), None)
	except:
		print("Error: an error occured during removing the street name")
# compute the lines between the points
def compute_lines():
	lines = {}
	try:
		for keys,vals in street_names.items():
			line = []
			for i in range(len(vals)-1):
				line.append(((vals[i][0], vals[i][1]), (vals[i+1][0], vals[i+1][1])))
			lines[keys] = line
	except:
		print("Error: an error occured during computing the line")
	return lines


def det(a, b):
	try:
		return a[0] * b[1] - a[1] * b[0]
	except:
		print("Error: an error occured during computing determination")
		return None

def intersect(l1, l2):
	try:
		xd = (l1[0][0] - l1[1][0], l2[0][0] - l2[1][0])
		yd = (l1[0][1] - l1[1][1], l2[0][1] - l2[1][1])
		div = det(xd, yd)
		if div == 0:
		   raise Exception('Error: lines do not intersect')

		d = (det(*l1), det(*l2))
		x = det(d, xd) / div
		y = det(d, yd) / div
		# check the condition that the intersection point is on the line or not
		if min(l1[0][0], l1[1][0]) > x or max(l1[0][0], l1[1][0]) < x or min(l2[0][0], l2[1][0]) > x or max(l2[0][0], l2[1][0]) < x:
			return None
		if min(l1[0][1], l1[1][1]) > y or max(l1[0][1], l1[1][1]) < y or min(l2[0][1], l2[1][1]) > y or max(l2[0][1], l2[1][1]) < y:
			return None
		return x, y
	except:
		return None

def add_edge(lines, dict, is_intersect):
	if lines in dict.keys():
		lst = dict[lines]
		if is_intersect not in lst:
			lst.append(is_intersect)
		temp_list = list(lines)

		if (temp_list[0] in lst) == False:
			lst.append(temp_list[0])
		if (temp_list[1] in lst) == False:
			lst.append(temp_list[1])
		dict[lines] = lst
	else:

		lst = list(lines)
		lst.append(is_intersect)
		dict[lines] = lst
	return dict

def compute_vertices():
	try:
		lines = compute_lines()
		st_arr = []
		for keys in lines.keys():
			st_arr.append(keys)
		vertices = set()
		edges = set()
		dict = {}
		for i in range(len(st_arr)-1):
			for j in range(i+1,(len(st_arr))):
				temp_set = set()
				temp_edge = set()
				temp_intersection = set()
				for k in range(len(lines[st_arr[i]])):
					temp_line_list = lines[st_arr[i]][k]
					for l in range(len(lines[st_arr[j]])):
						is_intersect = intersect(lines[st_arr[i]][k], lines[st_arr[j]][l])
						if is_intersect:
							add_edge(lines[st_arr[i]][k], dict, floating_point(is_intersect))
							add_edge(lines[st_arr[j]][l], dict, floating_point(is_intersect))


		for key in dict.keys():
			temp_list = dict[key]
			dict[key] = sorted(temp_list, key=lambda x: x[0])
		count = 0
		edges_str = ""
		temp_dict = {}
		for key in dict.keys():
			lst = []
			lst1 = []
			for i in range(len(dict[key])):
				if dict[key][i] in temp_dict.keys():
					lst.append([temp_dict[dict[key][i]], dict[key][i]])
				else:
					count+=1
					temp_dict[dict[key][i]] = count
					lst.append([count, dict[key][i]])
					lst1.append([count,dict[key][i]])


			for i in range(len(lst)-1):
				if lst[i][0] != lst[i+1][0]:
					edges_str = edges_str + '<'+str(lst[i][0])+','+str(lst[i+1][0])+'>,'
		if edges_str == "":
			edges_str = "E {}"
		else:
			edges_str = "E {"+edges_str[:-1] + '}'
		vertices_str = 'V ' + str(count)
		print(vertices_str)
		print(edges_str)
		sys.stdout.flush()
	except:
		print("Error: an error occured during printing the vertices and edges")

# Display vertices and edges
def gg():
	try:
		vertices = compute_vertices()
	except:
		print('Error: an error occured during printing the vertices and edges')

# validating the input
def validate_line(line):
	x = False
	y = False
	z = False
	try:
		matching_string = r"^(add|mod)\s\"[a-zA-Z]+(\s[a-zA-Z]+)*\"\s(\((-0*[1-9]\d*|\d+)\,(-0*[1-9]\d*|\d+)\))(\s\((-0*[1-9]\d*|\d+)\,(-0*[1-9]\d*|\d+)\))+$"
		x = re.match(matching_string, line)
		y = re.match(r"^gg$", line)
		z = re.match(r"^rm\s\"[a-zA-Z]+(\s[a-zA-Z]+)*\"$", line)

	except:
		return "Error: an error occuered during validating the line"
	return bool(x) or bool(y) or bool(z)

def main():
	while True:
		line = sys.stdin.readline()
		if len(line) == 0:
			break
		try:
			if validate_line(line):
				words = line.split()
				if words[0] == "add":
					add(words)
				elif words[0] == "mod":
					mod(words)
				elif words[0] == 'rm':
					rm(words)
				elif words[0] == 'gg':
					gg()
				else:
					continue
			else:
				print("Error: Please enter valid command")
		except:
			print("Error: an error occured")
		sys.stdout.flush()
	sys.exit(0)


if __name__ == "__main__":
    main()
