# Title
## Subtitle
*イタリック*や~~打ち消し線~~、日本語の文字も**大丈夫**！

1. リスト
2. アイテム2
3. アイテム3

[www.storange.jpへのリンクも](https://www.storange.jp)

![Automator Icon](Automator.png "Automator Icon")

```bash
export PATH=/usr/local/bin

for f in "$@"
do
 out=$(echo ${f} | sed -r "s/(.md)$/.pdf/g")
 md2pdf "${f}" "${out}"
done
```

| Left     | Center   | Right    |
| -------- |:--------:| --------:|
| Item 1x1 | Item 2x1 | Item 3x1 |
| Item 2x1 | Item 2x2 | Item 3x2 |
| Item 3x1 | Item 2x3 | Item 3x3 |

> Blockquote!!!

and hr ↓

---

インラインHTML ↓

<dl>
  <dt>01</dt>
  <dd>Hello</dd>

  <dt>02</dt>
  <dd>and Again</dd>
</dl>
