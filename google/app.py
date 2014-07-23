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
    import subprocess, os, sys
    print "request: ",request.form['search']

    origWD = os.getcwd() # remember our original working directory

    os.chdir(os.path.join(os.path.abspath(sys.path[0]), "indexer/output"))

    ## call date command ##
    p = subprocess.Popen(["./search","black"], stdout=subprocess.PIPE)

    ## Talk with date command i.e. read data from stdout and stderr. Store this info in tuple ##
    (output, err) = p.communicate()

    ## Wait for date to terminate. Get return returncode ##
    p_status = p.wait()
    print "Command output : ", output
    print "Command exit status/return code : ", p_status
    os.chdir(origWD) # g
    result = []
    # if len(output) > 10:
        # result = output[10:].split("content@:@")
    print output
    # for i, val in enumerate(result):
        # temp = val.split("@@@##@@@")
        # temp[1] = temp[1].strip('score@:@')
        # temp.append(temp[2].split('@@##@@id:')[1].strip("##$$##"))
        # result[i] = {'content': temp[0], 'pos': temp[3], 'other': temp[1], }

    return render_template('result.html', result=result)

if __name__ == '__main__':
    app.run()

