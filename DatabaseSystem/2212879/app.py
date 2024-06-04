from flask import Flask, abort, render_template, request, redirect, url_for, session, flash
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.exc import IntegrityError
from models import *
from utils import hash_password, check_password
from sqlalchemy import text
from sqlalchemy.exc import SQLAlchemyError

app = Flask(__name__)  # 初始化Flask应用
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:123456@localhost/Animation'
app.config['SECRET_KEY'] = 'your_secret_key'  # 配置Flask应用的密钥，用于会话管理
db.init_app(app)  # 初始化数据库连接

@app.before_request
def create_tables():
    db.create_all()  # 在第一次请求前创建所有数据库表

@app.route('/')
def index():

    return render_template('index.html')  # 渲染主页模板

# 用户注册路由
@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = hash_password(request.form['password'])  # 哈希用户密码
        email = request.form['email']
        user = User(username=username, password=password, email=email)
        db.session.add(user)  # 将新用户添加到数据库会话
        try:
            
            db.session.commit()  # 提交会话，保存新用户
            db.session.execute(text('CALL log_user_action(:user_id, :action)'), {'user_id': user.user_id, 'action': 'register'})
            db.session.commit()  # 提交会话，触发触发器
            return '''
            <script>
                alert('Registration successful. Please log in.');
                window.location.replace("/login"); // 
            </script>
            '''
        except IntegrityError:
            db.session.rollback()  # 如果提交失败，回滚事务
            return '''
            <script>
                alert('Username or email already exists.');
                window.location.replace("/register"); // 重定向
            </script>
            '''
    return render_template('register.html')  # 渲染注册页面


# 用户登录路由
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        user = User.query.filter_by(username=username).first()  # 根据用户名查找用户
        if user:
            if check_password(user.password, password):  # 校验密码
                session['user_id'] = user.user_id  # 将用户ID存储在会话中
                db.session.execute(text('CALL log_user_action(:user_id, :action)'), {'user_id': user.user_id, 'action': 'login'})
                db.session.commit()  # 提交用户操作日志
                return redirect(url_for('index'))  # 登录成功后重定向到用户主页
            else:
                return '''
                <script>
                    alert('Incorrect password. Please try again.');
                    window.location.replace("/login"); // 重定向
                </script>
                '''
        else:
            return '''
            <script>
                alert('Username does not exist. Please register.');
                window.location.replace("/register"); // 重定向
            </script>
            '''
    return render_template('login.html')  # 渲染登录页面


# 用户主页路由
@app.route('/user_home')
def user_home():
    if 'user_id' not in session:
        return redirect(url_for('login'))  # 如果用户未登录，重定向到登录页面
    user_id = session['user_id']
    user = User.query.get(user_id)  # 获取当前登录用户的信息
    reviews = Review.query.filter_by(user_id=user_id).all()  # 获取当前登录用户发布的评论

    anime = [Anime.query.get(favorite.anime_id) for favorite in user.favorites]    
    return render_template('user_home.html', user=user,  reviews=reviews,anime=anime)

# 用户登出路由
@app.route('/logout')
def logout():
    user_id = session['user_id']
    user = User.query.get(user_id)  # 获取当前登录用户的信息
    db.session.execute(text('CALL log_user_action(:user_id, :action)'), {'user_id': user.user_id, 'action': 'logout'})
    db.session.commit()  # 提交用户操作日志
    session.pop('user_id', None)  # 移除会话中的用户ID
    return redirect(url_for('index'))  # 重定向到主页

@app.route('/delete_user', methods=['POST'])
def delete_user():
    if 'user_id' in session:
        user_id = session['user_id']
        user = User.query.get(user_id)  # 获取要删除的用户
        if user:
            try:
                db.session.delete(user)  # 删除用户
                db.session.commit()  # 提交事务
                session.pop('user_id')  # 清除会话中的用户ID
                return '''
                <script>
                    alert('Your account has been deleted successfully.');
                    window.location.replace("/"); // 重定向到主页 
                </script>
                '''
            except Exception as e:
                db.session.rollback()  # 回滚事务
                flash(f'Error deleting user: {str(e)}')  # 显示错误信息
                return redirect(url_for('user_home'))
        else:
            return '''
            <script>
                alert('User not found.');
                window.location.replace("/"); // 重定向到主页
            </script>
            '''   
    else:
        return '''
        <script>
            alert('Please log in first.');
            window.location.replace("/"); // 重定向到主页
        </script>
        '''

    
# anime_list路由
@app.route('/anime_list')
def anime_list():
    animes = Anime.query.all()  # 查询所有动漫
    return render_template('anime_list.html', animes=animes)  # 渲染动漫列表页面并传递动漫列表数据


# 动漫详情路由
@app.route('/anime/<int:anime_id>')
def anime_detail(anime_id):
    anime = Anime.query.get_or_404(anime_id)  # 根据ID查找动漫
    reviews = Review.query.filter_by(anime_id=anime_id).all()  # 查找动漫的所有评论
    is_favorite = False
    if 'user_id' in session:
        user_id = session['user_id']
        user = User.query.get(user_id)
        is_favorite = anime in user.favorites
    if anime:
        studio = Studio.query.get(anime.studio_id)
        characters = Character.query.filter_by(anime_id=anime_id).all()
        voice_actors = []
        for character in characters:
            voice_actor = VoiceActor.query.get(character.voice_actor_id)
            voice_actors.append(voice_actor)
    return render_template('anime.html', anime=anime, reviews=reviews,is_favorite=is_favorite,studio=studio, characters=characters, voice_actors=voice_actors)  # 渲染动漫详情页面并传递动漫和评论信息

# 添加评论路由
@app.route('/anime/<int:anime_id>/review', methods=['GET', 'POST'])
def review(anime_id):
    if 'user_id' not in session:
        return redirect(url_for('login'))  # 如果用户未登录，重定向到登录页面
    if request.method == 'POST':
        user_id = session['user_id']
        rating = request.form.get('rating')
        comment = request.form.get('comment')

        try:
            review = Review(user_id=user_id, anime_id=anime_id, rating=rating, comment=comment)
            db.session.add(review)
            db.session.commit()
            return redirect(url_for('anime_detail', anime_id=anime_id))
        except SQLAlchemyError as e:
            error_info = str(e.__dict__['orig'])
            if 'Rating should be between 0 and 10' in error_info:
                flash('Rating should be between 0 and 10', 'error')
            else:
                flash('An error occurred while adding your review. Please try again.', 'error')
            return render_template('review.html', anime_id=anime_id)
    # GET 请求时渲染表单
    return render_template('review.html', anime_id=anime_id)

# 删除评论路由
@app.route('/delete_review/<int:review_id>', methods=['POST'])
def delete_review(review_id):
    if 'user_id' not in session:
        return redirect(url_for('login'))  # 如果用户未登录，重定向到登录页面

    review = Review.query.get(review_id)  # 获取要删除的评论对象
    if review:
        db.session.delete(review)  # 删除评论
        db.session.commit()  # 提交事务
        return '''
            <script>
                alert('Review deleted successfully.');
                window.location.replace("/user_home"); // 重定向到主页 
            </script>
            '''
    else:
        return '''
            <script>
                alert('Review not found.');
                window.location.replace("/user_home"); // 重定向到主页 
            </script>
            '''




# 收藏及取消收藏动漫路由
@app.route('/anime/<int:anime_id>/favorite')
def favorite(anime_id):
    if 'user_id' not in session:
        return redirect(url_for('login'))
    user_id = session['user_id']
    anime = Anime.query.get_or_404(anime_id)  # 获取动漫信息
    user = User.query.get(user_id)  # 获取当前登录用户的信息
    if anime in user.favorites:
        user.favorites.remove(anime)  
        db.session.commit()
    else:
        user.favorites.append(anime)
        db.session.commit()
    return redirect(url_for('anime_detail', anime_id=anime_id))  # 收藏成功后重定向到动漫详情页面

@app.route('/character<int:character_id>')
def character_detail(character_id):
    character = Character.query.get_or_404(character_id)
    voice_actor_name = character.voice_actor.name if character.voice_actor else None
    return render_template('character_detail.html', character=character, voice_actor_name=voice_actor_name)

@app.route('/voice_actor/<int:voice_actor_id>')
def voice_actor_detail(voice_actor_id):
    voice_actor = VoiceActor.query.get_or_404(voice_actor_id)
    return render_template('voice_actor_detail.html', voice_actor=voice_actor)

@app.route('/studio/<int:studio_id>')
def studio_detail(studio_id):
    studio = Studio.query.get(studio_id)
    if studio is None:
        abort(404)  # 如果找不到对应的公司，返回404错误
    return render_template('studio_detail.html', studio=studio)

# 用户修改邮件路由
@app.route('/change_email', methods=['GET', 'POST'])
def change_email():
    if 'user_id' not in session:
        return redirect(url_for('login'))  # 如果用户未登录，重定向到登录页面

    user_id = session['user_id']
    user = User.query.get(user_id)  # 获取当前登录用户信息

    if request.method == 'POST':
        new_email = request.form['new_email']
        user.email = new_email  # 更新用户邮箱
        db.session.commit()  # 提交会话，保存新邮箱
        return '''
            <script>
                alert('Email updated successfully.'); //显示成功信息
                window.location.replace("/user_home"); // 重定向
            </script>
            '''
    return render_template('change_email.html', user=user)  # 渲染修改邮箱页面

# 用户修改用户名路由
@app.route('/change_username', methods=['GET', 'POST'])
def change_username():
    if 'user_id' not in session:
        return redirect(url_for('login'))  # 如果用户未登录，重定向到登录页面

    user_id = session['user_id']
    user = User.query.get(user_id)  # 获取当前登录用户信息

    if request.method == 'POST':
        new_username = request.form['new_username']
        user.username = new_username  # 更新用户名
        db.session.commit()  # 提交会话，保存新用户名
        return '''
            <script>
                alert('Username updated successfully.');
                window.location.replace("/user_home"); // 重定向到
            </script>
            '''
    return render_template('change_username.html', user=user)  # 渲染修改用户名页面



@app.route('/search/title')
def search_anime_title():
    search_query = request.args.get('search_query')
    animes = Anime.query.filter(Anime.title.ilike(f'%{search_query}%')).all()
    return render_template('search_results.html', animes=animes, search_query=search_query)

@app.route('/search/genre')
def search_anime_genre():
    search_query = request.args.get('search_query')
    animes = Anime.query.filter(Anime.genre.ilike(f'%{search_query}%')).all()
    return render_template('search_results.html', animes=animes, search_query=search_query)

if __name__ == '__main__':
    app.run(debug=True)  # 以调试模式运行Flask应用
