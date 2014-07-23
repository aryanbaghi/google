# -*- coding: utf-8 -*-

from flask import Flask, abort, render_template
from flask import flash, request, session, redirect, url_for, g

app = Flask(__name__)
app.config.from_pyfile('config.py')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/result', methods=['GET', 'POST'])
def result():
	import subprocess
	print request.form['search']
	###
	# 
	# Your code here
	#
	###
	result = [
		{
			'content': 'first content',
			'pos': '100',
			'other': 'other info'
		},
		{
			'content': 'first content',
			'pos': '100',
			'other': 'other info'
		},
		{
			'content': 'first content',
			'pos': '100',
			'other': 'other info'
		},
	]

	return render_template('result.html', result=result)

if __name__ == '__main__':
    app.run()

