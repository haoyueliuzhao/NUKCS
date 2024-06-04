from datetime import datetime
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()  # 初始化SQLAlchemy


class Anime(db.Model):
    anime_id = db.Column(db.Integer, primary_key=True, autoincrement=True)  # 动漫ID，主键，自增长
    title = db.Column(db.Text, nullable=False)  # 标题，不能为空
    synopsis = db.Column(db.Text)  # 简介
    genre = db.Column(db.Text)  # 类型
    release_date = db.Column(db.Date)  # 发布日期
    episodes = db.Column(db.Integer)  # 集数
    status = db.Column(db.Text, nullable=False)  # 状态，不能为空
    rating = db.Column(db.Numeric(3, 2))  # 评分
    studio_id = db.Column(db.Integer)  # 制作公司ID
    cover_image = db.Column(db.Text)  # 封面图片

favorites_table = db.Table('favorites',
    db.Column('user_id', db.Integer, db.ForeignKey('user.user_id'), primary_key=True),
    db.Column('anime_id', db.Integer, db.ForeignKey('anime.anime_id'), primary_key=True)
    )

class User(db.Model):
    user_id = db.Column(db.Integer, primary_key=True, autoincrement=True)  # 用户ID，主键，自增长
    username = db.Column(db.String(255), unique=True, nullable=False)  # 用户名，唯一且不能为空
    password = db.Column(db.Text, nullable=False)  # 密码，不能为空
    email = db.Column(db.String(255), unique=True, nullable=False)  # 邮箱，唯一且不能为空
    joined_at = db.Column(db.DateTime, default=db.func.current_timestamp())  # 加入时间，默认为当前时间
    favorites = db.relationship('Anime', secondary=favorites_table, backref=db.backref('favorited_by', lazy='dynamic'))
class Review(db.Model):
    review_id = db.Column(db.Integer, primary_key=True, autoincrement=True)  # 评论ID，主键，自增长
    user_id = db.Column(db.Integer, db.ForeignKey('user.user_id'))  # 用户ID，外键关联 User 表
    anime_id = db.Column(db.Integer, db.ForeignKey('anime.anime_id'))  # 动漫ID，外键关联 Anime 表
    rating = db.Column(db.Numeric(3, 2))  # 评分
    comment = db.Column(db.Text)  # 评论
    created_at = db.Column(db.DateTime, default=db.func.current_timestamp())  # 创建时间，默认为当前时间

class Favorite(db.Model):
    user_id = db.Column(db.Integer, db.ForeignKey('user.user_id'), primary_key=True)
    anime_id = db.Column(db.Integer, db.ForeignKey('anime.anime_id'), primary_key=True)
    added_at = db.Column(db.DateTime, default=db.func.current_timestamp())
    

class Studio(db.Model):
    studio_id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.Text, nullable=False)
    studio_synopsis = db.Column(db.Text)
    founded_date = db.Column(db.Date)
    country = db.Column(db.Text)

class Character(db.Model):
    __tablename__ = 'Character'

    character_id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.Text, nullable=False)
    role = db.Column(db.Text)
    Character_synopsis = db.Column(db.Text)  # 简介
    anime_id = db.Column(db.Integer, db.ForeignKey('Anime.anime_id'))
    voice_actor_id = db.Column(db.Integer, db.ForeignKey('VoiceActor.voice_actor_id'))
    voice_actor = db.relationship('VoiceActor', backref='characters')

class VoiceActor(db.Model):
    __tablename__ = 'VoiceActor'

    voice_actor_id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.Text, nullable=False)
    VoiceActor_synopsis = db.Column(db.Text)  # 简介
    birthdate = db.Column(db.Date)
    nationality = db.Column(db.Text)

class UserLog(db.Model):
    log_id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    user_id = db.Column(db.Integer, db.ForeignKey('user.user_id'), nullable=False)
    action = db.Column(db.String(255), nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow, nullable=False)