%{
#include <stdio.h>
%}

%x comment1
%x comment2
%x quotes1
%x quotes2

%%

"/*"				BEGIN(comment1);
"//"				BEGIN(comment2);
\"				{ ECHO; BEGIN(quotes1); }
\'				{ ECHO; BEGIN(quotes2); }

<comment1>[^*]			;
<comment1>"*"+[^/]		;
<comment1>"*/"			BEGIN(INITIAL);

<comment2>[\\][ \t]*[\n]	;
<comment2>[^\n]			;
<comment2>[\n]			BEGIN(INITIAL);

<quotes1>[\\][\\]		{ ECHO; }
<quotes1>[\\]["]		{ ECHO; }
<quotes1>[^"]			{ ECHO; }
<quotes1>["]			{ ECHO; BEGIN(INITIAL); }

<quotes2>[\\][\\]               { ECHO; }
<quotes2>[\\][']                { ECHO; }
<quotes2>[^']                   { ECHO; }
<quotes2>[']                    { ECHO; BEGIN(INITIAL); }

[ \t]+				{ putchar(' '); } //skip many space/newlines to one

[\n]+				{ putchar('\n'); }

.				{ ECHO; }

%%

main() {
	yylex();	
}
