from .graph import make_graph, calc_freq
from natasha.markup import (
    format_json
)
wordgraph = make_graph(locals()["text"]) 
word_count = calc_freq(wordgraph)
format_json(word_count)
