sed -i -- 's/<head>/<head><link rel="stylesheet" href="..\/styles.css">/g' *
sed -i -- 's/<body lang="en">/<body lang="en"><div class="sidenav" id="common"><\/div><div class="main">/g' *
sed -i -- 's/<body lang="ru">/<body lang="ru"><div class="sidenav" id="common"><\/div><div class="main">/g' *
sed -i -- 's/<\/body>/<\/div><script type="text\/javascript" src="..\/accordion.js"><\/script><\/body>/g' *
sed -i -- 's/.png.png/.png/g' *
