
import re

def make_graph(text):
    rq = re.compile(r'[.!?]')
    sentenses = rq.split(text)
    list_sents = []
    for sent in sentenses:
        rx = re.compile(r'[-,]')
        sent = rx.sub('',sent)
        words = sent.split()
        list_sents.append(words)
    dict_words={}
    for k,words in enumerate(list_sents):
        for word in words:
            if word in dict_words:
                dict_words[word].append(k)
            else:
                dict_words[word]=[k]
    return dict_words
def sort_by_freq(item):
    return item[1]

def calc_freq(dict_words):
    lengths=[]
    for word in dict_words:
        lengths.append((word,len(dict_words[word])))

    lengths.sort(key=sort_by_freq, reverse=True)


    return lengths

