import argparse
parser = argparse.ArgumentParser(description='Process some strings.')
parser.add_argument('string',metavar='S',nargs='+',help='Convert string to ascii')
args = parser.parse_args()
String =  args.string[0]
for c in String:
	print ord(c)

