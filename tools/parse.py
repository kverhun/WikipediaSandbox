__author__ = 'Kostya'

import sys
import xml.etree.ElementTree
import re

def remove_description_from_reference(str):
    if '|' in str:
        return str[str.index('|')+1:]
    else:
        return str


def get_references_from_text(str):
    refs = [s[2:len(s)-2] for s in re.findall(R'\[\[[^\]]*\]\]', str)]
    refs = [remove_description_from_reference(ref) for ref in refs]
    return refs


def parse_xml(xml_file, max_pages_number):
    root = xml.etree.ElementTree.parse(xml_file).getroot()
    print(root)
    dict = {}

    current_page_index = 1
    page_number = len(root)

    number_of_references_found = 0

    for child in root:
        sys.stdout.write('\r')
        sys.stdout.write(str(current_page_index) + ' of ' + str(page_number))
        current_page_index += 1
        if current_page_index >= max_pages_number:
            break

        if child.tag == 'page':
            for page_attribute in child:
                if page_attribute.tag == 'title':
                    current_title = page_attribute.text
                if page_attribute.tag == 'revision':
                    for revision_attribute in page_attribute:
                        if revision_attribute.tag == 'text':
                            dict[current_title] = get_references_from_text(revision_attribute.text)
                            number_of_references_found += len(dict[current_title])

    print('\n')
    print('len(dict)', len(dict))
    print('number_of_references_found ', number_of_references_found)
    return dict


def write_parsing_result_to_csv(reference_list, file_name):
    with open(file_name, 'w') as f:
        ref_number = 0
        for article, references in reference_list.items():
            line = str(article)
            ref_number += len(references)
            for ref in references:
                line += ','
                line += str(ref)
            line += '\n'
            try:
                f.write(line)
            except:
                continue
        print('ref_number', ref_number)


def write_dictionary_to_csv(dict, file_name):
    with open(file_name, 'w') as f:
        for key, val in dict.items():
            line = str(key)
            line += ','
            line += str(val)
            line += '\n'
            try:
                f.write(line)
            except:
                continue


def transform_titles_to_indexes(reference_dictionary):
    index_to_string_map = {}
    string_to_index_map = {}
    references = {}
    current_index = 1

    for key, val in reference_dictionary.items():
        index_to_string_map[current_index] = key
        string_to_index_map[key] = current_index
        current_index += 1

    for key, val in reference_dictionary.items():
        try:
            idx = string_to_index_map[key]
            references[idx] = []
            for ref in val:
                try:
                    references[idx].append(string_to_index_map[ref])
                except:
                    continue
        except:
            continue

    return references, index_to_string_map, string_to_index_map


def __main__():
    xml_file = sys.argv[1]
    output_file = sys.argv[2]
    index_to_string_map_file = sys.argv[3]
    string_to_index_map_file = sys.argv[4]

    print('xml: ', xml_file)
    print('output: ', output_file)

    dict = parse_xml(xml_file, 2000)

    index_dict, index_to_string_map, string_to_index_map = transform_titles_to_indexes(dict)
    write_parsing_result_to_csv(index_dict, output_file)
    write_dictionary_to_csv(index_to_string_map, index_to_string_map_file)
    write_dictionary_to_csv(string_to_index_map, string_to_index_map_file)


__main__()