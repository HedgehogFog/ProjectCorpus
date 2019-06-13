from natasha import (
    NamesExtractor
)

extractor = NamesExtractor()
matches = extractor(locals()["text"])
spans = [_.span for _ in matches]
facts = [_.fact.as_json for _ in matches]
uniq_facts = []
uniq_count = []
for x in facts:
    if x not in uniq_facts:
        uniq_facts.append(x)
        i = uniq_facts.index(x)
        uniq_count.append(copy.deepcopy(uniq_facts[i]))
        uniq_count[i]['count'] = 1
    else:
        i = uniq_facts.index(x)
        uniq_count[i]['count'] += 1
    # #Форматируем результат в JSON
format_json(uniq_count)
