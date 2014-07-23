# -*- coding: utf-8 -*-

from flask import Flask, abort, render_template
from flask import flash, request, session, redirect, url_for, g
import unicodedata
import sys
reload(sys)
sys.setdefaultencoding("utf-8")


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
    p = subprocess.Popen(["./search",request.form['search']], stdout=subprocess.PIPE)
    ## Talk with date command i.e. read data from stdout and stderr. Store this info in tuple ##
    (output, err) = p.communicate()
    # print "-----------\n", output
    ## Wait for date to terminate. Get return returncode ##
    p_status = p.wait()
    # print "Command output : ", output
    # print "Command exit status/return code : ", p_status
    # os.chdir(origWD) # g
    result = []
    
    if output:
        if len(output) > 10:
            result = output[10:].split("content@:@")
    
    # print ">>>>", output


    for i, val in enumerate(result):
        temp = val.split("@@@##@@@")
        temp[1] = temp[1].strip('score@:@')
        temp.append(temp[2].split('@@##@@id:')[1].strip("##$$##"))
        a = temp[0].strip()
        a = repr(temp[0])
        # b = a.decode('utf-8')
        # print b
        # b = unicode(a, "utf8")
        # print type(b)*10
        # b = u''
        # print "0-0" * 10
        # print a
        # for i in a:
        #     print i
        #     b += unicode(i)
        # print b
        result[i] = {'content': a, 'pos': temp[3], 'other': temp[1], }



    # print resultunicode(my_string_variable, "utf8")
    # result = []
    # print 
    return render_template('result.html', result=result)

if __name__ == '__main__':
    app.run(host='0.0.0.0')

