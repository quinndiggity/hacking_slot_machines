__author__ = 'jaw'

subjects = ['astronomy',
            'english',
            'ancient_history',
            'maths',
            'animal_science',
            'physical_science',
            'famous_figures',
            'physical_geography',
            'natural_science',
            'spelling',
            'UK_history',
            'world_geography',
            'UK_geography',
            'world_religions',
            'european_geography',
            'history',
            'music',
            'religious_studies',
            'french',
            'citizenship']

keys = [['as1', 'as2','as3','as4','as5','as6','as7','as8','as9','as10'],
        ['en1', 'en2','en3','en4','en5','en6','en7','en8','en9','en10'],
        ['ah1', 'ah2','ah3','ah4','ah5','ah6','ah7','ah8','ah9','ah10'],
        ['ma1', 'ma2','ma3','ma4','ma5','ma6','ma7','ma8','ma9','ma10'],
        ['as1', 'as2','as3','as4','as5','as6','as7','as8','as9','as10'],
        ['ps1', 'ps2','ps3','ps4','ps5','ps6','ps7','ps8','ps9','ps10'],
        ['ff1', 'ff2','ff3','ff4','ff5','ff6','ff7','ff8','ff9','ff10'],
        ['pg1', 'pg2','pg3','pg4','pg5','pg6','pg7','pg8','pg9','pg10'],
        ['ns1', 'ns2','ns3','ns4','ns5','ns6','ns7','ns8','ns9','ns10'],
        ['sp1', 'sp2','sp3','sp4','sp5','sp6','sp7','sp8','sp9','sp10'],
        ['uh1', 'uh2','uh3','uh4','uh5','uh6','uh7','uh8','uh9','uh10'],
        ['wg1', 'wg2','wg3','wg4','wg5','wg6','wg7','wg8','wg9','wg10'],
        ['ug1', 'ug2','ug3','ug4','ug5','ug6','ug7','ug8','ug9','ug10'],
        ['wr1', 'wr2','wr3','wr4','wr5','wr6','wr7','wr8','wr9','wr10'],
        ['eg1', 'eg2','eg3','eg4','eg5','eg6','eg7','eg8','eg9','eg10'],
        ['hi1', 'hi2','hi3','hi4','hi5','hi6','hi7','hi8','hi9','hi10'],
        ['mu1', 'mu2','mu3','mu4','mu5','mu6','mu7','mu8','mu9','mu10'],
        ['rs1', 'rs2','rs3','rs4','rs5','rs6','rs7','rs8','rs9','rs10'],
        ['ci1', 'ci2','ci3','ci4','ci5','ci6','ci7','ci8','ci9','ci10']]

#define dictionary
dict = {}
dict_q = {}

for list in keys:
    subject = subjects[keys.index(list)]
    for key in list:
        q_number = list.index(key) + 1

        if q_number != 10:
            filename = subject + '_0' + str(q_number) + '.txt'

        else:
            filename = subject + '_' + str(q_number) + '.txt'

        question = []
        print filename

        try:
            with open('decrypted/%s' % filename) as infile:
                for line in infile:
                    question.append(line[:len(line)-1])
        except:
            print "Exception raised"
            question.append('N/A')

        answers = question[2:]
        q1 = question[0]

        dict[key] = answers
        dict_q[key] = q1+ '?'

print dict
print dict_q