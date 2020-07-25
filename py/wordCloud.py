import matplotlib.pyplot as plt # 数据可视化
import jieba # 词语分割
import wordcloud # 分词
from wordcloud import WordCloud, ImageColorGenerator, STOPWORDS # 词云，颜色生成器，停词器
import numpy as np # 科学计算
from PIL import Image # 处理图片

def ciyun():
    with open('少年中国说.txt', 'r', encoding='utf8') as f:
        textfile = f.read()
    # print(textfile)
    wordlist = jieba.lcut(textfile)
    space_list = ' '.join(wordlist)
    # print(space_list)
    background = np.array(Image.open('hs.jpg'))
    wc = WordCloud(width=474, height=841,
    background_color='white',
    mode='RGB',
    mask=background,
    max_words=500,
    stopwords=STOPWORDS.add('老年人'),
    font_path='zhunyuan.ttf',
    max_font_size=100,
    relative_scaling=0.6,
    random_state=50,
    scale=2).generate(space_list)

    image_color = ImageColorGenerator(background)
    wc.recolor(color_func=image_color)

    plt.imshow(wc)
    plt.axis('off')
    plt.show()
    wc.to_file("test_hs.jpg")


if __name__ == "__main__":
    ciyun()