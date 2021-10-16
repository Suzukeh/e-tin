---
customTheme: "solarslide"
theme: "solarized"
transition: "slide"
title: "電子ティンホイッスル"
---

#  電子ティンホイッスル

Arduinoに頼りまくって作る電子民族管楽器

Ⅲ類2年 鈴木 (Suzuke)

---

## Suzukeとは?

* 鈴木 輝
* 鈴木はたくさんいる <small>(要出典)</small>
* 「この鈴木」を指定するための、<br>ローカル内でたぶん固有な記号

--

## 自己紹介

* 鈴木輝 (Suzuke)
* 2年 Ⅲ類物理工学プログラム
* 死にかけツイッター：@suzukeh1
* https://suzukeh.github.io/mypage/<br>
できた！
* 逆RTAやりがち (今回もね)

今回が工研の活動として初めての制作(たぶん)

---

# 制作物

---

## 成れの果て

<img src="https://cdn.discordapp.com/attachments/843919142930874408/891508649611108362/image0.jpg" style="width: 60%; border: none; box-shadow:none;transform:rotate(0deg)"/>

<small>ラップの芯？ なにくっついてんの？ 配線クソか？ てかブレッドボード？</small> {.fragment .fade-up}

--

まあ、いろいろ(大切なもの含め)置いといて…

---

## やりたかったこと

大ざっぱに：**電子ティンホイッスル**が作りたい

<small>既存の電子管楽器例</small><br>
<img src="https://m.media-amazon.com/images/I/71tTTeWg3xL._AC_SL1500_.jpg" style="width: 5%; border: none; box-shadow:none;transform:rotate(0deg)"/> &emsp; <img src="https://static.roland.com/assets/images/products/gallery/aerophone_dr_gal.jpg" style="width: 8%; border: none; box-shadow:none;transform:rotate(0deg)"/>

--

そもそも
### ティンホイッスルとは

アイルランド周辺でよく用いられる民族楽器

非常に簡単な構造と奏法

イメージとしては、簡単すぎるリコーダー

--

民族楽器なので、電子化の波はあるものの未達<br>(バグパイプは既にある)

→じゃあこれは新規性アリだよね！！！ {.fragment .fade-up}

--

### 真面目な理由

* 音を抑える方法がほとんどなく、夜は練習できない
* 運指・奏法が簡単なので、DAW等の操作にも使えるかも？
* そもそも自分がキーボードできないので↑にメリットを感じた

---

## 目指したことと達成状況

* 軽押下圧のボタン or タッチセンサによる**素早い運指操作**<br>
→1MΩの抵抗を用いたタッチセンサで達成
* **MIDI送信**によるソフトシンセやDAWの操作
→原因不明で動作せず未達成 MIDI部単体は動作
* **息の強さ**でオクターブ操作<br>
→大気圧センサの値をもとに実装(if~しただけ)

--

### タッチセンサ

<img src="https://voltechno.com/blog/wp-content/uploads/2020/09/arduino-touch-006.jpg" style="width: 30%; border: none; box-shadow:none;transform:rotate(0deg)"/><br><small>https://voltechno.com/blog/post-20037/</small>

1MΩ側の抵抗をブレッドボード上に組み<br>人体に伸びる線を手元に持ってきた

--

### MIDI

**mocoLUFA**=Arduino UnoをUSB-MIDIデバイス化するファームウェア

単純にスケッチに書いて送信→Winダメ Macいける<br>怪しい雰囲気だができた

各センサ部の処理に合わせて、送信 →死

**!!!未解決!!!**

--

そもそもNanoとかLeonardなら、ファームウェアなしでUSBMIDIライブラリを使えばできるかも？

--

### ブレス
たぶん一番新規性がマシなところ

息の強さでオクターブ切り替えをするのがメインのものがなさそう (やろうと思えばできそうだが)

でも実装は本当に単純にifで区切っただけ {.fragment .fade-up}

--

物理的にも、これにBMP180を入れてあるだけ<br>
<img src="https://akizukidenshi.com/img/goods/C/M-12854.jpg" style="width: 50%; border: none; box-shadow:none;transform:rotate(0deg)"/>
<img src="https://www.packplus.jp/html/user_data/assets/img/product/1018128-1.jpg" style="width: 40%; border: none; box-shadow:none;transform:rotate(0deg)"/>

でも結構感度はいい {.fragment .fade-up}

---

##  まとめ

* (たぶん)存在していない電子楽器ができた
* 適したマイコンを使うべき
* MIDIは電子楽器としてさすがに必須
* タッチセンサは誤検知(というか個体差?)があるので、きちんとしたのを使った方がいい
* 大気圧センサ、優秀すぎ

でもまあ、意外と結構うまくいったと思います {.fragment .fade-up}

https://github.com/Suzukeh/e-tin {.fragment .fade-up}