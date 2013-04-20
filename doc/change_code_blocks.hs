import Text.Pandoc

main = toJsonFilter removeEmph
  where removeEmph (CodeBlock y xs) = RawBlock "latex" ("\\begin{minted}[]{cpp}\n" ++ xs ++ "\n\\end{minted}")
        removeEmph x         = x