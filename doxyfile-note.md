# DoxyFile 設定

## デフォルト設定変更内容

```shell
$ doxyfile -x

# Difference with default Doxyfile 1.9.8
PROJECT_NAME           = sunmoon
PROJECT_NUMBER         = 2026.2
PROJECT_BRIEF          = "astro calc package"
JAVADOC_AUTOBRIEF      = YES
JAVADOC_BANNER         = YES
BUILTIN_STL_SUPPORT    = YES
CPP_CLI_SUPPORT        = YES
USE_MDFILE_AS_MAINPAGE = README.md
HTML_FORMULA_FORMAT    = svg
USE_MATHJAX            = YES
GENERATE_LATEX         = NO
DOT_IMAGE_FORMAT       = svg
```

### JAVADOC_AUTOBRIEF = YES
 - JAVADOCコメント内の開始セクションを \@detail から \@brief に切り替える.
 - briefセクションの終了条件に "." "!" "?" 文字を加える.
   通常の終端条件は空行または他のセクションコマンドである.

### MULTILINE_CPP_IS_BRIEF = YES
 - "///"または"//!"の連続行コメント内の開始セクションを \@brief とし
   briefセクションの終了条件が固定となる.
   NOの場合は両者はJAVADOC_AUTOBRIEFの設定に従う.
 - "///"または"//!"の単行コメントの開始セクションは \@brief 固定かつ、briefセクションの終了条件も固定である.
   つまり本設定は連続行コメントを単行コメントと同じ扱いとするものである.

## END
