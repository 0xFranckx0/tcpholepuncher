#!/bin/bash

for i in $(find . -name \*.[c,h]);do
indent  --no-blank-lines-after-declarations \
        --blank-lines-after-procedures \
        --no-blank-lines-after-commas \
        --braces-on-if-line \
        --braces-on-struct-decl-line \
        --comment-indentation33 \
        --declaration-comment-column33 \
        --no-comment-delimiters-on-blank-lines \
        --cuddle-else \
        --continuation-indentation4 \
        --case-indentation10 \
        --line-comments-indentation0 \
        --declaration-indentation1 \
        --dont-format-first-column-comments \
        --indent-level8 \
        --parameter-indentation0 \
        --line-length80 \
        --break-after-boolean-operator \
        --ignore-newlines \
        --continue-at-parentheses \
        --no-space-after-function-call-names \
        --no-space-after-parentheses \
        --space-after-if \
        --space-after-for \
        --space-after-while \
        --no-space-after-casts \
        --dont-star-comments \
        --swallow-optional-blank-lines \
        --dont-format-comments \
        --else-endif-column33 \
        --space-special-semicolon \
        --tab-size8 \
        --indent-label1 \
        --no-tabs \
        $i
done