from distutils.util import strtobool
from os.path import isfile


def cond_negate(value, should_negate):
    if should_negate:
        if value[0] == "-":
            return value[1:]
        if float(value) != 0.0:
            return "-" + value
    return value

def cond_invert(value, should_negate):
    if should_negate:
        return str(1.0 - float(value))
    if value[-1] == "\n":
        return value[:-1]
    return value


def main():
    verticies = []
    tex_coords = []
    output = []

    valid = False

    while not valid:
        file_name = ""
        while not file_name:
            file_name = input("Enter a file path/name for the input .obj file: ")
        try:
            with open(file_name, "r") as file:
                valid = True

                flip = ""
                while True:
                    try:
                        flip = strtobool(input("Flip vertically [y/n]: ").lower())
                    except ValueError:
                        continue
                    break

                for line in file:
                    data = line.split(" ")

                    if data[0] == "v":
                        verticies.append([data[1],cond_negate(data[2], flip)])
                    elif data[0] == "vt":
                        tex_coords.append([data[1],cond_invert(data[2], flip)])
                    elif data[0] == "f":
                        for face in data[1:]:
                            v_idx = face.split("/")
                            vert = []
                            vert.extend(verticies[int(v_idx[0])-1])
                            vert.extend(tex_coords[int(v_idx[1])-1])
                            output.append(" ".join(vert))
                        output[-1] += "\n"
        except FileNotFoundError as _:
            pass

    file_name = ""
    overwrite = False
    while not file_name or not overwrite:
        file_name = input("Enter a file path/name for the output .sp file: ")

        if isfile(file_name):
            while True:
                try:
                    overwrite = strtobool(input("File "+file_name+" already exists, overwrite? [y/n]: ").lower())
                except ValueError:
                    continue
                break
        elif file_name:
            break

    with open(file_name, "w") as file:
        file.write("\n".join(output))


if __name__ == '__main__':
    main()
