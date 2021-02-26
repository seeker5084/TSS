// config...
const config = {
  wrapCode: false,
  indentSize: 2,
};

// just checking...
if ( !document.doctype ) completion( null );

// Getting current page source code
const html = new XMLSerializer().serializeToString( document );

// Making sure that the source code not to be interpreted as HTML
const sanitized = encodeURIComponent( html );

// code snippets...
const wordWrapCss = `
  pre,
  code {
    white-space: pre-wrap !important;
    word-wrap: break-word !important;
  }
`;

// Making "Source-Viewer" page
let viewer = `
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset="utf-8">
    <title>${ document.title }</title>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/prism/1.23.0/themes/prism.min.css" rel="stylesheet">
    <link href="https://cdnjs.cloudflare.com/ajax/libs/prism/1.23.0/plugins/autolinker/prism-autolinker.min.css" rel="stylesheet">
    <link href="https://cdnjs.cloudflare.com/ajax/libs/prism/1.23.0/plugins/line-numbers/prism-line-numbers.min.css" rel="stylesheet">
    <style>
      body {
        background-color: white;
        font-family: "Source Code Pro", Courier, monospace !important;
      }
      pre {
        border-radius: 5px;
      }
      ${ ( config.wrapCode ) ? wordWrapCss : '' }
    </style>
  </head>
  <body>
    <pre class="line-numbers"><code id="code" class="language-html"></code></pre>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.23.0/prism.min.js" data-manual></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.23.0/plugins/autolinker/prism-autolinker.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/prism/1.23.0/plugins/line-numbers/prism-line-numbers.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/js-beautify/1.13.4/beautify.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/js-beautify/1.13.4/beautify-css.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/js-beautify/1.13.4/beautify-html.min.js"></script>
    <script>
      let beautifyOptions = {
        'indent_size': 4,
      };
      let code = document.getElementById( 'code' );
      let raw = decodeURIComponent( \`${ sanitized }\` );
      code.textContent = html_beautify( raw, beautifyOptions );

      // Running Prism manually to avoid conflicts with "js-beautify" when showing large amount of source codes
      Prism.highlightElement( code );
    </script>
  </body>
  </html>
`;

// Returning "Source-Viewer" code to Shortcuts flow
completion( viewer );
