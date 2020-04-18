# 電池チェッカー
シンセに使えるかもと思って買ったOLEDがいつまでたっても日の目を見ないので使ってみました。  
[![](https://img.youtube.com/vi/ZLzqLWSBMcM/0.jpg)](https://www.youtube.com/watch?v=ZLzqLWSBMcM)
  
OLEDの端子は細ピンヘッダーに変更しています。  
デバッグ時はOLEDでリセットボタンを押していましたが、割れるといけないので  
カプトンテープに基板の切れ端を接着し､OLEDを支えるようにしています｡  
リセットスイッチをつけるほどでもないと思いましたが、配線エリアが余っていたので  
付けました。(ケーブルを抜き差しすれば済むので必須ではありません｡)  
![](https://github.com/mkomakonkon/electronic-work/blob/master/Battery%20checker/photos/Parts%20side%20BCHK.JPG)  
  
配線はこんな感じです。マイコン部の高さを抑えるためNanoを直付けしたので  
片面配線です｡  
左側の配線は1MΩのpull-downｘ２です。電池未実装の時Nanoの入力が安定しないので付けています。  
(部品面の写真だとどちらも隠れて見えないですね…)  
![](https://github.com/mkomakonkon/electronic-work/blob/master/Battery%20checker/photos/Wiring%20side%20BCHK.JPG)  
