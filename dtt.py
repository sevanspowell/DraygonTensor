import argparse

parser = argparse.ArgumentParser(description='Dragyon Tensor Toolbox - Tools for developers')
# parser.add_argument('integers', metavar='N', type=int, nargs='+',
#                     help='an integer for the accumulator')
# parser.add_argument('--sum', dest='accumulate', action='store_const',
#                     const=sum, default=max,
#                     help='sum the integers (default: find the max)')
parser.add_argument('type', choices=['project', 'build'])
parser.add_argument('build', choices=['Debug', 'Release', ''])

args = parser.parse_args()

print("~ Type: {}".format(args.type))
# print(args.accumulate(args.integers))
