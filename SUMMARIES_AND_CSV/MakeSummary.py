#!/usr/bin/python
import sys
import os
import re

def MakeSummary(test_slug, summary_file, test_file_path, name):
	device_line_written = False
	with(open(test_file_path)) as fulloutput:
		with (open(summary_file, 'a')) as summary_file:
			summary_file.write('Summary for {} with {}\n'.format(name, test_slug))
			for line in fulloutput:
				if 'Metrics for Device' in line and 'middle' not in line:
					summary_file.write(line)
					for i in range(0, 5):
						summary_file.write(fulloutput.next().strip('%'))
				elif 'NumberDevices = ' in line and not device_line_written:
					line = re.sub('_', '=', line)
					line = re.sub('LAMBA ', 'LAMBDA=', line)
					summary_file.write(line)
					device_line_written = True

			summary_file.write('\n\n\t***\n\n')

def MakeCSV(summary_file, csv_file):
	column_list = [ 'Number of Devices',
			'Lambda',
			'Mu',
			'Average Response for high priority',
			'Average Turnaround for high priority',
			'Miss Percetage for high priority',
			'Average Response for low priority',
			'Average Turnaround for low priority',
			'Miss Percetage for low priority' ]
	comma = ','
	with open(csv_file, 'a') as csv:
		with open(summary_file) as data:
			for line in data:
				if 'NumberDevices' in line:
					line = re.sub('[A-Z][a-z]+', '', line)
					line = re.sub('[a-z]+', '', line)
					line = line.strip()
					parts = line.split('=')
					column_list[0] = re.sub('[A-Z][a-z]', ' ', parts[1])
					column_list[1] = re.sub('[A-Z][a-z]', ' ', parts[2])
					column_list[2] = re.sub('[A-Z][a-z]', ' ', parts[3])
					column_list = strip_all(column_list, 0, 3)
				if 'highest' in line:
					line = re.sub('[A-Z][a-z]+', '', line)
					line = re.sub('[a-z]+', '', line)
					line = line.strip()

					column_list[3] = data.next().split(':')[1].strip()
					column_list[4] = data.next().split(':')[1].strip()
					consume = data.next()
					column_list[5] = data.next().split(':')[1].strip()
				if 'lowest' in line:
					line = re.sub('[A-Z][a-z]+', '', line)
					line = re.sub('[a-z]+', '', line)
					line = line.strip()

					column_list[6] =  data.next().split(':')[1].strip()
					column_list[7] =  data.next().split(':')[1].strip()
					consume = data.next()
					column_list[8] =  data.next().split(':')[1].strip()
					column_list = strip_all(column_list, 0, 3)
				if '***' in line:
					csv.write(get_row(column_list, comma))
				


def get_row(columns, sep):
	row = ''
	for col in columns:
		col = re.sub(',', '', col)
		col = col.strip()
		row += '{}{}'.format(col, sep)
	row = row[:-1] 
	if row[0] is ',':
		row = row[1:]
	row = row.strip()
	row = row + '\n'
	return row

def strip_all(lines, start, step):
	for i in range(start, start+step):
		lines[i].strip()
	return lines
def AddCsvHeader(filename):
	column_list = [ 'Number of Devices',
			'Lambda',
			'Mu',
			'Average Response for high priority',
			'Average Turnaround for high priority',
			'Miss Percetage for high priority',
			'Average Response for low priority',
			'Average Turnaround for low priority',
			'Miss Percetage for low priority' ]
	comma = ','

	with open (filename, 'a') as outfile:
		outfile.write(get_row(column_list, comma))


def main():
	test_slugs = ['QUEUE_SIZE_2_TESTS', 'QUEUE_SIZE_8_TESTS']
	test_tuples = [ [2, 2, 30], 
			[2, 2, 60], 
			[2, 2, 90], 
			[4, 2, 30],
			[4, 2, 60],
			[4, 2, 90],
			[8, 4, 30],
			[8, 6, 60],
			[8, 6, 90] ]
	for slug in test_slugs:	
		summary_file_name = 'Summary for {}'.format(slug)
		csv_file_name = 'CSV for {}'.format(slug)
		if os.path.isfile(summary_file_name):
			os.remove(summary_file_name)
		if os.path.isfile(csv_file_name):
			os.remove(csv_file_name)
		AddCsvHeader(csv_file_name)
		for test in test_tuples:
			test_file_name = 'DEVICES_{} LAMBA {} MU_{}'.format(test[0], test[1], test[2])
			test_file_path = '/home/u3/kgc0019/COMP3510/projects/lab3/{}/TEST_RESULTS/{}'.format(slug, test_file_name)
			MakeSummary(slug, summary_file_name, test_file_path, test_file_name)
			MakeCSV(summary_file_name, csv_file_name)



main()


