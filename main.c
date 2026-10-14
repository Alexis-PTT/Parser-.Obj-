#include <stdio.h>
#include <alexer.h>

#define ALEXER_IMPLEMENTATION
#include "alexer.h"

typedef enum {
    PUNCT_POINT,
    PUNCT_MINUS,
    PUNCT_DOUBLE_SLASH,
    COUNT_PUNCTS,
} Punct_Index;

static_assert(COUNT_PUNCTS == 3, "Amount of puncts have changed");
const char *puncts[COUNT_PUNCTS] = {
    [PUNCT_POINT]           = ".",
    [PUNCT_MINUS]           = "-",
    [PUNCT_DOUBLE_SLASH]    = "//",
};

typedef enum {
    KEYWORD_POINT,
    KEYWORD_NORMAL,
    KEYWORD_VERTEX,
    //TODO: see what it does in doc
    KEYWORD_S,
    KEYWORD_MTL,
    KEYWORD_G,
    KEYWORD_USE,
    COUNT_KEYWORDS,
} Keyword_Index;

static_assert(COUNT_KEYWORDS == 7, "Amount of keywords have changed");
const char *keywords[COUNT_KEYWORDS] = {
    [KEYWORD_POINT]      = "v",
    [KEYWORD_NORMAL]     = "vn",
    [KEYWORD_VERTEX]     = "f",
    [KEYWORD_G]          = "g",
    [KEYWORD_S]          = "s",
    [KEYWORD_MTL]        = "mtllib",
    [KEYWORD_USE]        = "usemtl",
};

const char *sl_comments[] = {
    "#"
};


int main()
{
    const char *file_path = "example/path"; // The file path is only needed for diagnostic message
    const char *content =
    "# Blender v2.78 (sub 0) OBJ File: '12.blend\n"
    "# www.blender.org\n"
    "mtllib Goldfish_01.mtl\n"
    "g Goldfish_01_Cube.058\n"
    "v 1.791817 2.124454 2.120096\n"
    "vn -0.4091 -0.8730 0.2654\n"
    "usemtl FF5722\n"
    "s off\n"
    "f 31//1 19//1 5//1 26//1\n"
    "";
    Alexer l = alexer_create(file_path, content, strlen(content));
    l.puncts = puncts;
    l.puncts_count = ALEXER_ARRAY_LEN(puncts);
    l.keywords = keywords;
    l.keywords_count = ALEXER_ARRAY_LEN(keywords);
    l.sl_comments = sl_comments;
    l.sl_comments_count = ALEXER_ARRAY_LEN(sl_comments);
    Alexer_Token token = {0};
    bool end=false;
    uint64_t start_tokens[]={ALEXER_ID(ALEXER_KEYWORD,KEYWORD_VERTEX),
                            ALEXER_ID(ALEXER_KEYWORD,KEYWORD_MTL)};
    uint64_t start_double[]={ALEXER_ID(ALEXER_PUNCT,PUNCT_MINUS),
                            ALEXER_ID(ALEXER_INT,0)};
    while (!end){
        alexer_get_token(&l, &token );
        //printf("%lu",token.id);
        if (!alexer_expect_one_of_ids(&l,token,start_tokens,2)) return 1;
        switch (ALEXER_KIND(token.id))
        {
            case  ALEXER_KEYWORD:
                l.diagf(token.loc, "COMMANDE", "%s: %.*s", alexer_kind_name(ALEXER_KIND(token .id)),
                        token.end - token.begin, token.begin);
                Alexer_Token cmdToken=token;
                switch (ALEXER_KIND(cmdToken.id)){
                    case KEYWORD_VERTEX:
                        break;
                    case KEYWORD_G:
                        break;
                    case KEYWORD_MTL:
                        alexer_get_token(&l, &token );
                        if (!alexer_expect_id(&l,token,ALEXER_SYMBOL)) return 1;
                        l.diagf(token.loc, "PARA 1", "%s: %.*s", alexer_kind_name(ALEXER_KIND(token .id)),
                                token.end - token.begin, token.begin);
                        alexer_get_token(&l, &token );
                        if (!alexer_expect_id(&l,token,ALEXER_ID(ALEXER_PUNCT,PUNCT_POINT))) return 1;
                        l.diagf(token.loc, "verif point ", "%s: %.*s", alexer_kind_name(ALEXER_KIND(token .id)),
                                token.end - token.begin, token.begin);
                        alexer_get_token(&l, &token );
                        if (!alexer_expect_id(&l,token, ALEXER_SYMBOL)) return 1;
                        l.diagf(token.loc, "verif format ", "%s: %.*s", alexer_kind_name(ALEXER_KIND(token .id)),
                                token.end - token.begin, token.begin);
                        break;
                    case KEYWORD_USE:
                        break;
                    case KEYWORD_POINT:
                        if (!alexer_expect_id(&l,token,PUNCT_MINUS)) return 1;
                        l.diagf(token.loc, "COMMANDE", "%s: %.*s", alexer_kind_name(ALEXER_KIND(token .id)),
                                token.end - token.begin, token.begin);
                        bool negation=false;
                        double x =0.0F;
                        alexer_get_token(&l, &token );
                        double y =0.0F;
                        double z =0.0F;
                        break;
                    case KEYWORD_S:
                        break;
                    case KEYWORD_NORMAL:
                        break;
                    default:
                        break;
                }
                break;
            case  ALEXER_END:
                end = true;
                break;
            default:
                l.diagf(token.loc, "DONNEE", "%s: %.*s", alexer_kind_name(ALEXER_KIND(token .id)),
                       token.end - token.begin, token.begin);
                break;
        }
        end = true;
    }
    //while (alexer_get_token(&l, &t)) {
    //    l.diagf(t.loc, "INFO", "%s: %.*s", alexer_kind_name(ALEXER_KIND(t.id)), t.end - t.begin, t.begin);
    //}
    //if (!alexer_expect_id(&l, t, ALEXER_END)) return 1;

    return 0;
}