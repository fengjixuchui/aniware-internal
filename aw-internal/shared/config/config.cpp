#include "../shared.hpp"

namespace config
{
	std::string_view m_name = "aniware_cfg";
	std::string_view m_config = "default.cfg";

	std::vector<item_t>& get_items( )
	{
		static std::vector<item_t> items;
		return items;
	}

	item_t& get_item( const uint32_t index )
	{
		return get_items( ).at( index );
	}

	int does_item_exist( const hash_t name )
	{
				for ( auto i = 0u; i < get_items( ).size( ); i++ )
			if ( get_items( ).at( i ).m_name == name )
				return i;

		return -1;
	}

	void save( )
	{
		auto config_path = std::filesystem::path( m_name ) /= m_config;

		std::filesystem::create_directory( config_path.parent_path( ) );
		nlohmann::json out;

		for ( auto& item : get_items( ) )
		{
			nlohmann::json cur;

			cur[ "identifier" ] = item.m_name;
			cur[ "type" ] = item.m_type;

			switch ( item.m_type )
			{
				case CT_HASH( "bool" ):
				{
					cur[ "inner" ] = item.get<bool>( );
					break;
				}
				case CT_HASH( "float" ):
				{
					cur[ "inner" ] = item.get<float>( );
					break;
				}
				case CT_HASH( "int" ):
				{
					cur[ "inner" ] = item.get<int>( );
					break;
				}
				case CT_HASH( "col_t" ):
				{
					auto col = item.get<col_t>( );
					nlohmann::json sub;

					sub.push_back( col.r( ) );
					sub.push_back( col.g( ) );
					sub.push_back( col.b( ) );
					sub.push_back( col.a( ) );

					cur[ "inner" ] = sub.dump( );
					break;
				}
				case CT_HASH( "std::vector<int>" ):
				{ 
					auto vec = item.get<std::vector<int>>( );
					nlohmann::json sub;

					for ( auto& cur : vec )
						sub.push_back( cur );

					cur[ "inner" ] = sub.dump( );
					break;
				}
				case CT_HASH( "std::vector<float>" ):
				{
					auto vec = item.get<std::vector<float>>( );
					nlohmann::json sub;

					for ( auto cur : vec )
						sub.push_back( cur );

					cur[ "inner" ] = sub.dump( );
					break;
				}
				case CT_HASH( "std::vector<bool>" ):
				{
					auto vec = item.get<std::vector<bool>>( );
					nlohmann::json sub;

					for ( auto cur : vec )
						sub.push_back( static_cast< int >( cur ) );

					cur[ "inner" ] = sub.dump( );
					break;
				}
				default:
					break;
			}

			out.push_back( cur );
		}

		std::ofstream out_file( config_path, std::ios::out | std::ios::trunc );
		out_file << out.dump( 4 );
		out_file.close( );
	}

	void load( )
	{
		auto config_path = std::filesystem::path( m_name ) /= m_config;
		std::filesystem::create_directory( config_path.parent_path( ) );

		nlohmann::json in;

		std::ifstream config_file( config_path, std::ios::in );
		config_file >> in;
		config_file.close( );

		for ( auto& item : in )
		{
			auto item_index = does_item_exist( item[ "identifier" ].get<hash_t>( ) );
			if ( item_index < 0 )
				continue;

			auto& cur_item = get_items( ).at( item_index );

			switch ( item[ "type" ].get<hash_t>( ) )
			{
				case CT_HASH( "bool" ):
				{
					cur_item.set<bool>( item[ "inner" ].get<bool>( ) );
					break;
				}
				case CT_HASH( "float" ):
				{
					cur_item.set<float>( item[ "inner" ].get<float>( ) );
					break;
				}
				case CT_HASH( "int" ):
				{
					cur_item.set<int>( item[ "inner" ].get<int>( ) );
					break;
				}
				case CT_HASH( "col_t" ):
				{
					auto vec = nlohmann::json::parse( item[ "inner" ].get<std::string>( ) );

					cur_item.set<col_t>( col_t(
						vec.at( 0 ).get<int>( ),
						vec.at( 1 ).get<int>( ),
						vec.at( 2 ).get<int>( ),
						vec.at( 3 ).get<int>( )
					) );

					break;
				}
				case CT_HASH( "std::vector<int>" ):
				{
					auto vec = nlohmann::json::parse( item[ "inner" ].get<std::string>( ) );
					auto& item_vec = cur_item.get<std::vector<int>>( );

					for ( auto i = 0u; i < vec.size( ); i++ )
					{
						if ( i < item_vec.size( ) )
							item_vec.at( i ) = vec.at( i ).get<int>( );
					}
					break;
				}
				case CT_HASH( "std::vector<float>" ):
				{
					auto vec = nlohmann::json::parse( item[ "inner" ].get<std::string>( ) );
					auto& item_vec = cur_item.get<std::vector<float>>( );

					for ( auto i = 0u; i < vec.size( ); i++ )
					{
						if ( i < item_vec.size( ) )
							item_vec.at( i ) = vec.at( i ).get<float>( );
					}
					break;
				}
				case CT_HASH( "std::vector<bool>" ):
				{
					auto vec = nlohmann::json::parse( item[ "inner" ].get<std::string>( ) );
					auto& item_vec = cur_item.get<std::vector<bool>>( );

					for ( auto i = 0u; i < vec.size( ); i++ )
					{
						if ( i < item_vec.size( ) )
							item_vec.at( i ) = vec.at( i ).get<int>( );
					}
					break;
				}
				default:
					break;
			}
		}
	}

	void set_config_file( const std::string_view config )
	{
		m_config = config;
	}

	std::string_view get_config_file( )
	{
		return m_config;
	}

	void set_config_directory( const std::string_view name )
	{
		m_name = name;
	}

	std::string_view get_config_directory( )
	{
		return m_name;
	}
}